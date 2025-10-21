#!/usr/bin/perl
use strict;
use warnings;
use Getopt::Long;
use utf8;
binmode(STDOUT, ':utf8');

# 脚本功能：分析单个tcmalloc heap profile文件，按当前分配字节降序排列
# 并自动解析堆栈地址为符号信息
# 用法: perl heap_analyzer.pl --file=file.heap [--min-bytes=1024] [--min-allocs=10] [--top=20]

my $heap_file = "";
my $min_bytes = 1048576;   # 最小字节数阈值，过滤太小的分配
my $min_allocs = 100;  # 最小分配次数，过滤太少的分配
my $top_n = 0;        # 只显示前N个分配，0表示显示全部
my $help = 0;
my $resolve_symbols = 1;  # 是否解析符号，默认开启

# 全局变量存储模块信息
my %module_cache = ();  # 缓存模块基地址信息
my $module_cache_loaded = 0;  # 标记是否已加载模块信息
my %symbols_cache_by_addr = ();  # 缓存符号信息

GetOptions(
    "file=s" => \$heap_file,
    "min-bytes=i" => \$min_bytes,
    "min-allocs=i" => \$min_allocs,
    "top=i" => \$top_n,
    "resolve!" => \$resolve_symbols,
    "help|h"   => \$help
) or die "命令行参数错误!\n";

if ($help || !$heap_file) {
    print_usage();
    exit(0);
}

# 检查文件是否存在
die "错误: 文件 '$heap_file' 不存在!\n" unless -f $heap_file;

print "=== Tcmalloc Heap Profile 分析工具 ===\n";
print "分析文件: $heap_file\n";
print "最小字节阈值: $min_bytes bytes\n";
print "最少分配阈值: $min_allocs 次\n";
print "显示前N个: " . ($top_n ? $top_n : "全部") . "\n";
print "符号解析: " . ($resolve_symbols ? "开启" : "关闭") . "\n";

# 如果启用符号解析，从heap文件中加载模块信息
if ($resolve_symbols) {
    load_module_cache_from_heap($heap_file);
}

# 解析heap profile文件
my %heap_data = parse_heap_file($heap_file);

# 分析并显示结果
analyze_heap(\%heap_data);

sub print_usage {
    print <<EOF;
用法: perl heap_analyzer.pl [选项]

选项:
  --file=FILE        指定heap profile文件
  --min-bytes=N      只显示字节数大于等于N的分配 (默认: 1MB)
  --min-allocs=N     只显示次数大于等于N的分配 (默认: 100)
  --top=N            只显示前N个分配 (默认: 全部)
  --resolve          启用符号解析 (默认)
  --no-resolve       禁用符号解析
  --help, -h         显示此帮助信息

示例:
  perl heap_analyzer.pl --file=heap.heap
  perl heap_analyzer.pl --file=heap.heap --min-bytes=1048576 --top=20

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
            if ($line =~ /^([0-9a-fA-F]+)-([0-9a-fA-F]+)\s+\S+\s+\S+\s+\S+\s+\S+\s+(.+)$/) {
                my $start_addr = $1;
                my $end_addr = $2;
                my $module_path = $3;
                
                # 提取模块文件名
                my $module_name = $module_path;
                $module_name =~ s/^.*[\\\/]//;  # 去掉路径，只保留文件名
                
                # 计算模块大小
                my $size = bighex('0x' . $end_addr) - bighex('0x' . $start_addr);
                
                # 存储模块信息
                $module_cache{$module_name} = {
                    base_address => bighex('0x' . $start_addr),
                    size => $size,
                    full_path => $module_path
                };
                
                $module_count++;
                
                # 也存储完整路径作为key，以防只有完整路径的情况
                $module_cache{$module_path} = {
                    base_address => bighex('0x' . $start_addr),
                    size => $size,
                    full_path => $module_path
                };
            }
        }
    }
    
    close $fh;
    
    if ($module_count > 0) {
        print "  - 从heap文件加载了 $module_count 个模块的地址信息\n";
        $module_cache_loaded = 1;
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

sub analyze_heap {
    my ($heap_ref) = @_;
    my %heap_data = %$heap_ref;
    
    print "\n=== 开始堆内存分析 ===\n";
    
    # 过滤和排序分配记录
    my @allocations = ();
    my $total_bytes = 0;
    my $total_filtered_bytes = 0;
    my $total_allocs = 0;
    my $total_filtered_allocs = 0;
    
    for my $stack (keys %heap_data) {
        my $data = $heap_data{$stack};
        $total_bytes += $data->{current_bytes};
        $total_allocs += $data->{current_allocs};
        
        # 应用过滤条件
        if ($data->{current_bytes} >= $min_bytes && $data->{current_allocs} >= $min_allocs) {
            push @allocations, {
                stack => $stack,
                data => $data
            };
            $total_filtered_bytes += $data->{current_bytes};
            $total_filtered_allocs += $data->{current_allocs};
        }
    }
    
    # 按当前分配字节数降序排序
    @allocations = sort { $b->{data}->{current_bytes} <=> $a->{data}->{current_bytes} } @allocations;
    
    # 应用top限制
    if ($top_n > 0 && @allocations > $top_n) {
        @allocations = @allocations[0..$top_n-1];
    }
    
    # 显示结果
    print "\n-------------------【内存分配排名】-------------------\n";
    print "总计: " . scalar(keys %heap_data) . " 个调用栈\n";
    print "过滤前: " . format_bytes($total_bytes) . " (" . $total_allocs . " 次分配)\n";
    print "过滤后: " . format_bytes($total_filtered_bytes) . " (" . $total_filtered_allocs . " 次分配)\n";
    print "显示: " . scalar(@allocations) . " 个调用栈\n\n";
    
    for my $i (0 .. $#allocations) {
        my $alloc = $allocations[$i];
        my $data = $alloc->{data};
        my $stack = $alloc->{stack};
        
        print sprintf("--- 排名 #%d ---\n", $i + 1);
        print_allocation_info($data, $stack);
        print "\n";
    }
    
    # 显示统计信息
    if (@allocations > 0) {
        print "\n=== 统计信息 ===\n";
        print sprintf("最大分配: %s (#1)\n", format_bytes($allocations[0]{data}{current_bytes}));
        print sprintf("最小显示分配: %s (#%d)\n", 
            format_bytes($allocations[-1]{data}{current_bytes}), scalar(@allocations));
        print sprintf("平均分配大小: %s\n", 
            format_bytes($total_filtered_bytes / scalar(@allocations)));
    }
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
        if (exists($symbols_cache_by_addr{$addr})) {
            my $symbol_info = $symbols_cache_by_addr{$addr};
            print " → $symbol_info";
        } else {
            my $symbol_info = resolve_address_symbol($addr);
            if ($symbol_info) {
                $symbols_cache_by_addr{$addr} = $symbol_info;
                print " → $symbol_info";
            }
        }
        print "\n";
    }
    
    # 如果解析失败，提供备用建议
    print "\n  如果符号解析失败，可以手动使用:\n";
    my $addr_str = join(" ", @addresses[0..min(2, $#addresses)]);
    print "  使用WinDbg: ln $addr_str\n";
}

# 以下函数与原脚本相同，保持完整性
sub bighex {
    my $hex = shift;

    my $high_part = qr/[0-9a-fA-F]{0,8}/;
    my $low_part = qr/[0-9a-fA-F]{8}/;
    my ($high, $low) = $hex =~ /^0x($high_part)($low_part)$/;
    if (not defined($low)) {
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
    
    # 检查是否在Windows环境下
    my $os = get_os_type();
    if ($os ne 'windows') {
        return undef;  # 目前只支持Windows
    }
    
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
    return resolve_with_symquery($module_name, $relative_addr);
}

sub find_module_for_address {
    my ($address) = @_;
    
    foreach my $module_name (keys %module_cache) {
        my $base_addr = $module_cache{$module_name}->{base_address};
        my $size = $module_cache{$module_name}->{size};
        my $end_addr = $base_addr + $size;
        
        if ($address >= $base_addr && $address < $end_addr) {
            my $relative_addr = $address - $base_addr;
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
    my $result = `symquery.exe -e "$module_name" -a "$hex_addr" 2>nul`;
    chomp $result;
    
    if ($result && $result !~ /^\s*$/ && $result !~ /error/i) {
        # 简化输出，只保留主要信息
        $result =~ s/\s+/ /g;
        return "[[$module_name+0x" . sprintf("%X", $relative_addr) . "]$result";
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
    return 'windows';
}

sub print_windows_conversion_tips {
    my (@addresses) = @_;
    
    print "  Windows下的地址转换建议:\n\n";
    
    # 方法1: 使用symquery
    print "  方法1 - 使用symquery:\n";
    print "  # 首先获取模块基地址，然后计算相对地址\n";
    print "  symquery -e <模块名.dll> -a <相对地址>\n\n";
    
    # 方法2: 使用WinDbg
    print "  方法2 - 使用WinDbg:\n";
    for my $addr (@addresses) {
        print "  ln $addr  # 查看地址 $addr 对应的符号\n";
    }
    print "\n";
}

sub print_linux_conversion_tips {
    my (@addresses) = @_;
    
    print "  Linux下的地址转换建议:\n\n";
    
    # 传统的addr2line方法
    print "  方法1 - 使用addr2line:\n";
    print "  addr2line -e your_program -f -C -i " . join(" ", @addresses) . "\n\n";
    
    # 使用gdb
    print "  方法2 - 使用gdb:\n";
    print "  gdb your_program\n";
    for my $addr (@addresses) {
        print "  (gdb) info line *$addr\n";
    }
    print "\n";
}

1;