# Stat
[![Stat Build Workflow](https://github.com/gamemann/Stat/actions/workflows/build.yml/badge.svg)](https://github.com/gamemann/Stat/actions/workflows/build.yml)

## Description
A small project that allows you to gather statistics (counters) from files on the file system or command outputs. This was designed for Linux.

This is useful for retrieving the incoming/outgoing packets or bytes per second on a network interface from the file system or from the output of a command such as `ethtool`.

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
--cmd => The command to execute and retrieve output from.
--sep => The separator to apply to the command's output.
--key => The key to search for when separating the command output.
```

**Note** - If you want to receive another counter such as outgoing (TX) packets, you can set the file to pull the count from with the `-p` (or `--path`) flag. For example:

```
gstat --path /sys/class/net/ens18/statistics/tx_packets
```

### Command Argument
You may use a combination of the `--cmd`, `--sep`, and `--key` arguments to retrieve a counter from a command's output such as `ethtool`.

For example, take a look at the following output from `ethtool`.

```bash
$ sudo ethtool -S enp1s0

NIC statistics:
     rx_queue_0_packets: 901268
     rx_queue_0_bytes: 211930005
     rx_queue_0_drops: 0
     rx_queue_0_xdp_packets: 0
     rx_queue_0_xdp_tx: 0
     rx_queue_0_xdp_redirects: 0
     rx_queue_0_xdp_drops: 0
     rx_queue_0_kicks: 14
     rx_queue_1_packets: 1237084
     rx_queue_1_bytes: 469671713
     rx_queue_1_drops: 0
     rx_queue_1_xdp_packets: 0
     rx_queue_1_xdp_tx: 0
     rx_queue_1_xdp_redirects: 0
     rx_queue_1_xdp_drops: 0
     rx_queue_1_kicks: 19
     tx_queue_0_packets: 16508781
     tx_queue_0_bytes: 1528281628
     tx_queue_0_xdp_tx: 0
     tx_queue_0_xdp_tx_drops: 0
     tx_queue_0_kicks: 6598201
     tx_queue_0_tx_timeouts: 0
     tx_queue_1_packets: 16154602
     tx_queue_1_bytes: 1348764220
     tx_queue_1_xdp_tx: 0
     tx_queue_1_xdp_tx_drops: 0
     tx_queue_1_kicks: 6051221
     tx_queue_1_tx_timeouts: 0
```

If I want the `rx_queue_0_packets` value used as the counter (starting at `901268`), you would execute the following command.

```bash
sudo gstat --cmd "ethtool -S enp1s0" --sep ":" --key "rx_queue_0_packets"
```

Each line from the command's output is trimmed of white-spaces.

## Credits
* [Christian Deacon](https://github.com/gamemann)
