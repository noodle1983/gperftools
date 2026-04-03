#!/usr/bin/perl
#
#exp: perl parse_symbols.pl -m a.exe.memmap 0x7ffa654b2bc5
#
use strict;
use warnings;
use Getopt::Long;
use File::Spec;
use Cwd qw(getcwd);

my $memmap_file = '';
my $help = 0;

GetOptions(
  'm|memmap=s' => \$memmap_file,
  'h|help'     => \$help,
) or die usage();

if ($help || !$memmap_file || !@ARGV) {
  print usage();
  exit($help ? 0 : 1);
}

die "错误: memmap 文件不存在: $memmap_file\n" unless -f $memmap_file;

my @modules = parse_memmap($memmap_file);
if (!@modules) {
  die "错误: 未在 $memmap_file 中解析到可执行模块映射\n";
}

my $resolver = detect_resolver();

print "=== Symbol Parser ===\n";
print "memmap: $memmap_file\n";
print "modules: " . scalar(@modules) . "\n";
print "resolver: $resolver->{desc}\n\n";

my $idx = 0;
for my $addr (@ARGV) {
  $idx++;
  my ($ok, $msg) = resolve_one_address($addr, \@modules, $resolver);
  print sprintf("#%d %s\n", $idx, $msg);
  print "\n";
}

exit 0;

sub usage {
  return <<'EOF';
用法:
  perl parse_symbols.pl -m <memmap文件> <addr1> [addr2 ...]

示例:
  perl parse_symbols.pl -m SIOT.exe.memmap 0x7ffa25f9bb0b 0x7ffa25f9a741

说明:
  1) 地址必须是十六进制，支持 0x 前缀。
  2) 模块选择优先级:
     - 当前目录同名模块 (basename)
     - memmap 里的模块全路径
  3) 解析器优先级:
     - 当前目录 pstack/symquery.exe
     - PATH 里的 symquery.exe
     - (非 Windows) addr2line
EOF
}

sub parse_memmap {
  my ($file) = @_;
  open my $fh, '<', $file or die "无法打开 $file: $!\n";

  my @mods;
  my $in_map = 0;
  while (my $line = <$fh>) {
    chomp $line;
    if ($line =~ /^MAPPED_LIBRARIES:\s*$/) {
      $in_map = 1;
      next;
    }
    next if !$in_map;
    next if $line =~ /^\s*$/;

    if ($line =~ /^([0-9A-Fa-f]+)-([0-9A-Fa-f]+)\s+(\S+)\s+(\S+)\s+\S+\s+\S+\s+(.+)$/) {
      my ($start_hex, $end_hex, $perm, $off_hex, $path) = ($1, $2, $3, $4, $5);
      next unless $perm =~ /x/;

      my $start = bighex("0x$start_hex");
      my $end = bighex("0x$end_hex");
      my $off = bighex("0x$off_hex");
      my $size = $end - $start;
      next if $size <= 0;

      my $base_name = basename($path);
      my $chosen = choose_module_path($path, $base_name);

      push @mods, {
        start => $start,
        end => $end,
        size => $size,
        file_offset => $off,
        full_path => $path,
        base_name => $base_name,
        chosen_path => $chosen,
      };
    }
  }

  close $fh;

  @mods = sort { $a->{start} <=> $b->{start} } @mods;
  return @mods;
}

sub basename {
  my ($path) = @_;
  $path =~ s/^.*[\\\/]//;
  return $path;
}

sub choose_module_path {
  my ($full_path, $base_name) = @_;
  my $cwd = getcwd();

  my $local = File::Spec->catfile($cwd, $base_name);
  if (-f $local) {
    return $local;
  }

  return $full_path;
}

sub detect_resolver {
  my $os = $^O;
  my $cwd = getcwd();

  if ($os =~ /^MSWin/) {
    my $local = File::Spec->catfile($cwd, 'pstack', 'symquery.exe');
    if (-f $local) {
      return { type => 'symquery', cmd => $local, desc => $local };
    }

    my $where = `where symquery.exe 2>nul`;
    if ($? == 0 && $where =~ /\S/) {
      my ($first) = split /\r?\n/, $where;
      return { type => 'symquery', cmd => $first, desc => $first };
    }

    return { type => 'none', cmd => '', desc => 'unavailable (symquery.exe not found)' };
  }

  my $which_sym = `which symquery 2>/dev/null`;
  if ($? == 0 && $which_sym =~ /\S/) {
    chomp $which_sym;
    return { type => 'symquery', cmd => $which_sym, desc => $which_sym };
  }

  my $which_addr2line = `which addr2line 2>/dev/null`;
  if ($? == 0 && $which_addr2line =~ /\S/) {
    chomp $which_addr2line;
    return { type => 'addr2line', cmd => $which_addr2line, desc => $which_addr2line };
  }

  return { type => 'none', cmd => '', desc => 'unavailable (no resolver found)' };
}

sub parse_hex_addr {
  my ($s) = @_;
  return undef if !defined $s;
  return bighex($s) if $s =~ /^0x[0-9A-Fa-f]+$/;
  return bighex("0x$s") if $s =~ /^[0-9A-Fa-f]+$/;
  return undef;
}

sub resolve_one_address {
  my ($addr_text, $mods_ref, $resolver) = @_;

  my $addr = parse_hex_addr($addr_text);
  if (!defined $addr) {
    return (0, "address=$addr_text\n  error: 非法地址格式");
  }

  my $mod = find_module($addr, $mods_ref);
  if (!$mod) {
    return (0, sprintf("address=%s (0x%X)\n  module: <not found>", $addr_text, $addr));
  }

  my $rel = $addr - $mod->{start} + $mod->{file_offset};
  my $rel_hex = sprintf('0x%X', $rel);

  my $symbol = resolve_symbol($resolver, $mod->{chosen_path}, $rel_hex);

  my $result = "";
  $result .= sprintf("address=%s (0x%X)\n", $addr_text, $addr);
  $result .= sprintf("  module: %s\n", $mod->{full_path});
  $result .= sprintf("  file: %s\n", $mod->{base_name});
  $result .= sprintf("  using: %s\n", $mod->{chosen_path});
  $result .= sprintf("  relative: %s\n", $rel_hex);
  $result .= sprintf("  symbol: %s", $symbol);

  return (1, $result);
}

sub find_module {
  my ($addr, $mods_ref) = @_;
  for my $m (@$mods_ref) {
    if ($addr >= $m->{start} && $addr < $m->{end}) {
      return $m;
    }
  }
  return undef;
}

sub resolve_symbol {
  my ($resolver, $module_path, $rel_hex) = @_;

  if ($resolver->{type} eq 'symquery') {
    my $cmd = $resolver->{cmd};
    my @targets = ($module_path);
    my $base = basename($module_path);
    push @targets, $base if $base ne $module_path;

    my $out = '';
    my $success = 0;
    for my $target (@targets) {
      $out = `"$cmd" -e "$target" -a "$rel_hex" 2>&1`;
      chomp $out;
      $out =~ s/\r//g;
      $out =~ s/\s+/ /g;
      if ($? == 0 && $out ne '') {
        $success = 1;
        last;
      }
    }
    if ($success) {
      return $out;
    }
    if ($out eq '') {
      return "<symquery failed: no output>";
    }
    return "<symquery failed: $out>";
  }

  if ($resolver->{type} eq 'addr2line') {
    my $cmd = $resolver->{cmd};
    my $out = `"$cmd" -e "$module_path" -f -C "$rel_hex" 2>&1`;
    chomp $out;
    $out =~ s/\r//g;
    if ($? == 0 && $out ne '') {
      $out =~ s/\n/ | /g;
      return $out;
    }
    return $out eq '' ? "<addr2line failed: no output>" : "<addr2line failed: $out>";
  }

  return '<resolver unavailable>';
}

sub bighex {
  my ($hex) = @_;
  my $high_part = qr/[0-9a-fA-F]{0,8}/;
  my $low_part = qr/[0-9a-fA-F]{1,8}/;
  my ($high, $low) = $hex =~ /^0x($high_part)($low_part)$/;
  if (!defined $low) {
    die "not hex: [$hex]\n";
  }
  $high = '' if !defined $high;
  my $h = $high eq '' ? 0 : hex("0x$high");
  my $l = hex("0x$low");
  return ($h << (4 * length($low))) + $l if length($low) < 8;
  return ($h << 32) + $l;
}
