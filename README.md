# Stat
![Stat Build Workflow](https://github.com/gamemann/Stat/actions/workflows/c.yml/badge.svg)
## Description
A small project that allows you to gather statistics (integers/counts) from files on the file system. This was designed for Linux.

This is useful for retrieving the incoming/outgoing packets per second or incoming/outgoing bytes per second on a network interface.

## Building Program
You can simply use `make` to build this program. The `Makefile` uses `clang` to compile the program.

```
apt-get install clang # (Debian/Ubuntu-based systems)
yum install devtoolset-7 llvm-toolset-7 llvm-toolset-7-clang-analyzer llvm-toolset-7-clang-tools-extra # (CentOS/Others)
make
```

You may use `make install` to copy the `gstat` executable to your `$PATH` via `/usr/bin`.

**Note** - We use `gstat` instead of `stat` due to other common packages.

## Command Line Usage
General command line usage can be found below.

```
gstat [-i <interface> --pps --bps --path <path> -c <"kbps" or "mbps" or "gbps"> --custom <integer>]
--pps => Set path to RX packet path.
--bps => Set path to RX byte path.
-p --path => Use count (integer) from a given path on file system.
-i --dev => The name of the interface to use when setting --pps or --bps.
-c --convert => Convert to either "kbps", "mbps", or "gbps".
--custom => Divides the count value by this much before outputting to stdout.
--interval => Use this interval (in microseconds) instead of one second.
--count -n => Maximum amount of times to request the counter before stopping program (0 = no limit).
--time -t => Time limit (in seconds) before stopping program (0 = no limit).
```

**Note** - If you want to receive another counter such as outgoing (TX) packets, you can set the file to pull the count from with the `-p` (or `--path`) flag. For example:

```
gstat --path /sys/class/net/ens18/statistics/tx_packets
```

## Credits
* [Christian Deacon](https://github.com/gamemann)
