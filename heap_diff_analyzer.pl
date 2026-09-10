#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
use utf8;
binmode(STDOUT, ':utf8');

# 脚本功能：分析两个tcmalloc heap profile文件的差异，找出新增的内存分配
# 并自动解析堆栈地址为符号信息
# 用法: perl heap_diff_analyzer.pl --before=file1.heap --after=file2.heap [--min-bytes=1024] [--process=program_name]

my $before_file = "";
my $after_file = "";
my $min_bytes = 16;   # 最小字节数阈值，过滤太小的分配
my $min_allocs = 10;  # 最小分配次数，过滤太少的分配
my $help = 0;
my $resolve_symbols = 1;  # 是否解析符号，默认开启

# 全局变量存储模块信息
my %module_cache = ();  # 缓存模块基地址信息
my $module_cache_loaded = 0;  # 标记是否已加载模块信息

my %symbols_cache_by_addr = ();  # 缓存模块基地址信息

GetOptions(
    "before=s" => \$before_file,
    "after=s"  => \$after_file,
    "min-bytes=i" => \$min_bytes,
    "min-allocs=i" => \$min_allocs,
    "resolve!" => \$resolve_symbols,
    "help|h"   => \$help
) or die "命令行参数错误!\n";

if ($help || !$before_file || !$after_file) {
    print_usage();
    exit(0);
}

# 检查文件是否存在
die "错误: 文件 '$before_file' 不存在!\n" unless -f $before_file;
die "错误: 文件 '$after_file' 不存在!\n" unless -f $after_file;

print "=== Tcmalloc Heap Profile 差异分析工具 ===\n";
print "前文件: $before_file\n";
print "后文件: $after_file\n";
print "最小字节阈值: $min_bytes bytes\n";
print "最少分配阈值: $min_allocs 次\n";
print "符号解析: " . ($resolve_symbols ? "开启" : "关闭") . "\n";

# 如果启用符号解析，从heap文件中加载模块信息
if ($resolve_symbols) {
    load_module_cache_from_heap($after_file);
}

# 解析heap profile文件
my %before_data = parse_heap_file($before_file);
my %after_data = parse_heap_file($after_file);

# 分析差异
analyze_diff(\%before_data, \%after_data);

sub print_usage {
    print <<EOF;
用法: perl heap_diff_analyzer.pl [选项]

选项:
  --before=FILE     指定前一个heap profile文件
  --after=FILE      指定后一个heap profile文件
  --min-bytes=N     只显示字节数大于等于N的分配 (默认: 16)
  --min-allocs=N    只显示次数大于等于N的分配 (默认: 10)
  --process=NAME    指定进程名，用于地址解析
  --resolve         启用符号解析 (默认)
  --no-resolve      禁用符号解析
  --help, -h        显示此帮助信息

示例:
  perl heap_diff_analyzer.pl --before=heap1.heap --after=heap2.heap
  perl heap_diff_analyzer.pl --before=heap1.heap --after=heap2.heap --min-bytes=1024 --process=myprogram

EOF
}

sub load_module_cache_from_heap {
    my ($heap_file) = @_;
    
    print "正在从heap文件加载模块信息...\n";
    
    open(my $fh, '<', $heap_file) or die "无法打开文件 '$heap_file': $!\n";
    
    my $in_mapped_libraries = 0;
    my $module_count = 0;
    
    while (my $line = <$fh>) {
        chomp $line;
        
        # 检查是否到达MAPPED_LIBRARIES部分
        if ($line =~ /^MAPPED_LIBRARIES:/) {
            $in_mapped_libraries = 1;
            next;
        }
        
        # 如果在MAPPED_LIBRARIES部分，解析模块信息
        if ($in_mapped_libraries) {
            # 格式: 起始地址-结束地址 权限 偏移 设备 inode 模块路径
            # 示例: 7ffa00000000-7ffa00021000 r--p 00000000 00:00 0 /path/to/module.dll
            if ($line =~ /^([0-9a-fA-F]+)-([0-9a-fA-F]+)\s+(\S+)\s+(\S+)\s+\S+\s+\S+\s+(.+)$/) {
                my $start_addr = $1;
                my $end_addr = $2;
                my $permission = $3;
                my $file_offset = $4;
                my $module_path = $5;

                next unless $permission =~ /x/;
                
                # 提取模块文件名
                my $module_name = $module_path;
                $module_name =~ s/^.*[\\\/]//;  # 去掉路径，只保留文件名
                
                # 计算模块大小
                my $size = bighex('0x' . $end_addr) - bighex('0x' . $start_addr);
                
                # 存储模块信息
                $module_cache{$module_name} = {
                    base_address => bighex('0x' . $start_addr),
                    file_offset => bighex('0x' . $file_offset),
                    size => $size,
                    full_path => $module_path
                };
                
                $module_count++;
                
                # 也存储完整路径作为key，以防只有完整路径的情况
                # $module_cache{$module_path} = {
                #     base_address => bighex('0x' . $start_addr),
                #     size => $size,
                #     full_path => $module_path
                # };
            }
        }
    }
    
    close $fh;
    
    if ($module_count > 0) {
        print "  - 从heap文件加载了 $module_count 个模块的地址信息\n";
        $module_cache_loaded = 1;
        
        # 打印加载的模块信息（可选，用于调试）
        print "  加载的模块:\n";
        foreach my $module (sort keys %module_cache) {
            my $info = $module_cache{$module};
            printf "    %s: 0x%x (大小: 0x%x bytes, 偏移: 0x%x)\n", 
                   $module, $info->{base_address}, $info->{size},  $info->{file_offset};
        }
    } else {
        print "  - 警告: 未在heap文件中找到模块映射信息\n";
        print "  - 请确保heap文件包含MAPPED_LIBRARIES部分\n";
    }
}

sub parse_heap_file {
    my ($filename) = @_;
    my %data;
    
    print "解析文件: $filename\n";
    
    open my $fh, '<', $filename or die "无法打开文件 '$filename': $!\n";
    
    my $line_count = 0;
    my $record_count = 0;
    my $in_mapped_libraries = 0;
    
    while (my $line = <$fh>) {
        $line_count++;
        chomp $line;
        
        # 跳过MAPPED_LIBRARIES部分（已经在load_module_cache_from_heap中处理）
        if ($line =~ /^MAPPED_LIBRARIES:/) {
            $in_mapped_libraries = 1;
            next;
        }
        
        if ($in_mapped_libraries) {
            # 跳过模块映射行，它们已经处理过了
            next;
        }
        
        # 跳过文件头
        next if $line =~ /^heap profile:/;
        
        # 解析分配记录行
        # 格式: 当前分配数: 当前字节数 [累计分配数: 累计字节数] @ 调用栈...
        if ($line =~ /^\s*(\d+):\s+(\d+)\s+\[\s*(\d+):\s+(\d+)\]\s+@\s*(.*)$/) {
            my ($current_allocs, $current_bytes, $total_allocs, $total_bytes, $stack) = 
               ($1, $2, $3, $4, $5);
            
            # 清理调用栈地址，去掉多余空格
            $stack =~ s/\s+/ /g;
            $stack =~ s/^\s+|\s+$//g;
            
            # 以调用栈为key存储数据
            $data{$stack} = {
                current_allocs => $current_allocs,
                current_bytes  => $current_bytes,
                total_allocs   => $total_allocs,
                total_bytes    => $total_bytes
            };
            $record_count++;
        }
        # 处理没有调用栈信息的记录
        elsif ($line =~ /^\s*(\d+):\s+(\d+)\s+\[\s*(\d+):\s+(\d+)\]\s+@\s*$/) {
            my ($current_allocs, $current_bytes, $total_allocs, $total_bytes) = 
               ($1, $2, $3, $4);
            
            my $stack = "<无调用栈信息>";
            $data{$stack} = {
                current_allocs => $current_allocs,
                current_bytes  => $current_bytes,
                total_allocs   => $total_allocs,
                total_bytes    => $total_bytes
            };
            $record_count++;
        }
    }
    
    close $fh;
    
    print "  - 处理了 $line_count 行，找到 $record_count 个唯一的分配记录\n";
    
    return %data;
}

sub analyze_diff {
    my ($before_ref, $after_ref) = @_;
    my %before = %$before_ref;
    my %after = %$after_ref;
    
    print "\n=== 开始差异分析 ===\n";
    
    my @new_allocations = ();
    my @increased_allocations = ();
    my $total_new_bytes = 0;
    my $total_increased_bytes = 0;
    
    # 查找新增的和增长的分配
    for my $stack (keys %after) {
        if (!exists $before{$stack}) {
            # 完全新增的调用栈
            my $bytes = $after{$stack}->{current_bytes};
            my $allocs = $after{$stack}->{current_allocs};
            if ($bytes >= $min_bytes && $allocs >= $min_allocs) {
                push @new_allocations, {
                    stack => $stack,
                    data => $after{$stack}
                };
                $total_new_bytes += $bytes;
            }
        } else {
            # 已有调用栈的分配增长
            my $before_bytes = $before{$stack}->{current_bytes};
            my $after_bytes = $after{$stack}->{current_bytes};
            my $diff_bytes = $after_bytes - $before_bytes;
            my $allocs = $after{$stack}->{current_allocs};
            
            if ($diff_bytes > $min_bytes && $allocs >= $min_allocs) {
                push @increased_allocations, {
                    stack => $stack,
                    before_data => $before{$stack},
                    after_data => $after{$stack},
                    diff_bytes => $diff_bytes
                };
                $total_increased_bytes += $diff_bytes;
            }
        }
    }
    
    # 按字节数排序
    @new_allocations = sort { $b->{data}->{current_bytes} <=> $a->{data}->{current_bytes} } @new_allocations;
    @increased_allocations = sort { $b->{diff_bytes} <=> $a->{diff_bytes} } @increased_allocations;
    
    # 显示结果
    print "\n-------------------【新增分配】-------------------\n";
    print "总计: " . scalar(@new_allocations) . " 个新调用栈，共 " . format_bytes($total_new_bytes) . "\n\n";
    
    for my $i (0 .. $#new_allocations) {
        my $alloc = $new_allocations[$i];
    
		my $data = $alloc->{data};
		my $stack = $alloc->{stack};
		
        print sprintf("--- 新增 #%d ---\n", $i + 1);
        print_allocation_info($data, $stack);
        print "\n";
    }
    
    print "\n-------------------【增长分配】-------------------\n";
    print "总计: " . scalar(@increased_allocations) . " 个调用栈分配增长，共 " . format_bytes($total_increased_bytes) . "\n\n";
    
    for my $i (0 .. $#increased_allocations) {
        my $alloc = $increased_allocations[$i];
		my $data = $alloc->{after_data};
		my $stack = $alloc->{stack};
		
        print sprintf("--- 增长 #%d ---\n", $i + 1);
        print sprintf("变化: %s -> %s (增加 %s)\n",
            format_bytes($alloc->{before_data}->{current_bytes}),
            format_bytes($alloc->{after_data}->{current_bytes}),
            format_bytes($alloc->{diff_bytes})
        );
		
        print_allocation_info($data, $stack);
        print "\n";
    }
    
    # 总体统计
    print "\n=== 总体统计 ===\n";
    print sprintf("新增内存分配: %d 个调用栈，%s\n", 
        scalar(@new_allocations), format_bytes($total_new_bytes));
    print sprintf("增长内存分配: %d 个调用栈，增加 %s\n", 
        scalar(@increased_allocations), format_bytes($total_increased_bytes));
    print sprintf("总计净增长: %s\n", 
        format_bytes($total_new_bytes + $total_increased_bytes));
}

sub print_allocation_info {
    my ($data, $stack) = @_;
	
    print sprintf("当前分配: %d 次，%s\n",
        $data->{current_allocs}, format_bytes($data->{current_bytes}));
    print sprintf("累计分配: %d 次，%s\n",
        $data->{total_allocs}, format_bytes($data->{total_bytes}));
    
    # 显示详细的调用栈信息
    print "调用栈:\n";
    if ($stack eq "<无调用栈信息>") {
        print "  (无调用栈信息)\n";
    } else {
        my @addresses = split /\s+/, $stack;
        
        # 如果启用了符号解析且模块信息已加载
        if ($resolve_symbols && $module_cache_loaded && @addresses > 0) {
            print_stack_with_symbols(@addresses);
        } else {
            # 只显示地址
            for my $i (0 .. $#addresses) {
                print sprintf("  #%-2d %s\n", $i, $addresses[$i]);
            }
            
            # 提供手动解析建议
            if (@addresses > 0) {
                print "\n  地址转换建议:\n";
                print_address_conversion_tips(@addresses[0..min(2, $#addresses)]);
            }
        }
    }
}

sub print_stack_with_symbols {
    my (@addresses) = @_;
        
    for my $i (0 .. $#addresses) {
        my $addr = $addresses[$i];
        print sprintf("  #%-2d %s", $i, $addr);
        
        # 解析单个地址的符号信息
		if (exists($symbols_cache_by_addr{$addr})){
			my $symbol_info = $symbols_cache_by_addr{$addr};
			print " → $symbol_info";
		}
		else{
			my $symbol_info = resolve_address_symbol($addr);
			if ($symbol_info) {
				$symbols_cache_by_addr{$addr} = $symbol_info;
				print " → $symbol_info";
			}
		}
        print "\n";
    }
}

sub bighex {
    my $hex = shift;

	my $high_part = qr/[0-9a-fA-F]{0,8}/;
    my $low_part = qr/[0-9a-fA-F]{8}/;
    my ($high, $low) = $hex =~ /^0x($high_part)($low_part)$/;
	if (not defined($low)){
		print "not hex: [$hex]\n";
		exit -1;
	}
    return hex("0x$low") + (hex("0x$high") << 32);
}

sub to_bighex {
    my $decimal = shift;

    my $high = $decimal >> 32;
    my $low  = $decimal & 0xFFFFFFFF;

    return sprintf("%08x%08x", $high, $low);
}

sub resolve_address_symbol {
    my ($address) = @_;
    
    # 将地址转换为数值
    my $addr_num;
    if ($address =~ /^0x([0-9a-fA-F]+)$/) {
        $addr_num = bighex($address);
    } else {
        return undef;
    }
    
    # 查找该地址属于哪个模块
    my ($module_name, $relative_addr) = find_module_for_address($addr_num);
    if (!$module_name) {
        return "[未知模块]";
    }
    
    # 使用symquery解析符号
    my $os = get_os_type();
    if ($os eq 'windows') {
        return resolve_with_symquery($module_name, $relative_addr);
    } else {
        return resolve_with_addr2line($module_name, $relative_addr);
    }
}

sub find_module_for_address {
    my ($address) = @_;
    
    foreach my $module_name (keys %module_cache) {
        my $base_addr = $module_cache{$module_name}->{base_address};
        my $size = $module_cache{$module_name}->{size};
        my $file_offset = $module_cache{$module_name}->{file_offset};
        my $end_addr = $base_addr + $size;
        
        if ($address >= $base_addr && $address < $end_addr) {
            my $relative_addr = $address - $base_addr + $file_offset;
            return ($module_name, $relative_addr);
        }
    }
    
    return (undef, undef);
}

sub resolve_with_symquery {
    my ($module_name, $relative_addr) = @_;
    
    # 检查symquery是否可用
    my $symquery_check = `where symquery.exe 2>nul`;
    if (!$symquery_check) {
        return "[$module_name+0x" . sprintf("%X", $relative_addr) . "]";
    }
    
    # 调用symquery解析符号
    my $hex_addr = sprintf("0x%X", $relative_addr);
    my $result = `symquery.exe -e "$module_name" -f -a "$hex_addr" 2>nul`;
    chomp $result;
    
    if ($result && $result !~ /^\s*$/ && $result !~ /error/i) {
        # 简化输出，只保留主要信息
        $result =~ s/\s+/ /g;
        return "[[$module_name+0x" . sprintf("%X", $relative_addr) . "]$result";
    } else {
        return "[$module_name+0x" . sprintf("%X", $relative_addr) . "]";
    }
}

sub resolve_with_addr2line {
    my ($module_name, $relative_addr) = @_;
    
    # 检查symquery是否可用
    my $symquery_check = `which addr2line 2>nul`;
    if (!$symquery_check) {
        return "[$module_name+0x" . sprintf("%X", $relative_addr) . "]";
    }
    
    # 调用symquery解析符号
    my $hex_addr = sprintf("0x%X", $relative_addr);
    my $result = `addr2line -e "$module_name" "$hex_addr" 2>nul`;
    chomp $result;
    
    if ($result && $result !~ /^\s*$/ && $result !~ /error/i) {
        # 简化输出，只保留主要信息
        $result =~ s/\s+/ /g;
        return "[[$module_name+0x" . sprintf("%X", $relative_addr) . "] $result";
    } else {
        return "[$module_name+0x" . sprintf("%X", $relative_addr) . "]";
    }
}

sub format_bytes {
    my ($bytes) = @_;
    
    if ($bytes >= 1024 * 1024 * 1024) {
        return sprintf("%.2f GB", $bytes / (1024 * 1024 * 1024));
    } elsif ($bytes >= 1024 * 1024) {
        return sprintf("%.2f MB", $bytes / (1024 * 1024));
    } elsif ($bytes >= 1024) {
        return sprintf("%.2f KB", $bytes / 1024);
    } else {
        return "$bytes bytes";
    }
}

sub min {
    my ($a, $b) = @_;
    return $a < $b ? $a : $b;
}

sub print_address_conversion_tips {
    my (@addresses) = @_;
    my $os = get_os_type();
    
    if ($os eq 'windows') {
        print_windows_conversion_tips(@addresses);
    } else {
        print_linux_conversion_tips(@addresses);
    }
}

sub get_os_type {
    my $os = $^O;

    if ($os =~ /^MSWin/) {  # 匹配 MSWin32 或 MSWin64
        return 'windows';
    } elsif ($os eq 'darwin') {
        return 'macOS';
    } elsif ($os eq 'linux') {
        return 'linux';
    } else {
        return $os;
        # 其他可能的值：
        # solaris, aix, freebsd, openbsd, netbsd, dec_osf, irix, hpux, ...
    }
}

sub print_windows_conversion_tips {
    my (@addresses) = @_;
    my $addr_str = join(" ", @addresses);
    
    print "  Windows下的地址转换建议:\n\n";
    
    # 方法1: 使用symquery
    print "  方法1 - 使用symquery:\n";
    print "  # 首先获取模块基地址，然后计算相对地址\n";
    print "  symquery -e <模块名.dll> -f -a <相对地址>\n\n";
    
    # 方法2: 使用WinDbg
    print "  方法2 - 使用WinDbg:\n";
    for my $addr (@addresses) {
        print "  ln $addr  # 查看地址 $addr 对应的符号\n";
    }
    print "\n";
}

sub print_linux_conversion_tips {
    my (@addresses) = @_;
    my $addr_str = join(" ", @addresses);
    
    print "  Linux下的地址转换建议:\n\n";
    
    # 传统的addr2line方法
    print "  方法1 - 使用addr2line:\n";
    print "  addr2line -e your_program -f -C -i $addr_str\n\n";
    
    # 使用gdb
    print "  方法2 - 使用gdb:\n";
    print "  gdb your_program\n";
    for my $addr (@addresses) {
        print "  (gdb) info line *$addr\n";
    }
    print "\n";
}
