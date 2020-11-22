# Linux Network Statistics
## Description
A small repository showing how to gather simple network statistics using C on Linux. In this repository, we show how to retrieve the amount of packets per second and bytes per second (supporting common and custom conversions) on a specific interface.

## Building Programs
You can simply use `make` to build these programs. The `Makefile` uses `clang` to compile these programs.

```
apt-get install clang # (Debian/Ubuntu-based systems)
yum install devtoolset-7 llvm-toolset-7 llvm-toolset-7-clang-analyzer llvm-toolset-7-clang-tools-extra # (CentOS/Others)
make
```

You can make specific programs using `make pps` or `make bps`.

## Packets Per Second
This small program retrieves the amount of packets per second going in on an interface. You must specify the interface name via the `-i` or `--dev` arguments.

Example:

```
./pps -i ens18
```

## Bytes Per Second
This small program retrieves the amount of bytes per second going in on an interface. You must specify the interface name via the `-i` or `--dev` arguments.

Optionally, you may also set conversions with the `-c` or `--convert` arguments that accepts `kbps`, `mbps`, or `gbps`. Additionally, you can also specify the `--custom` argument which will divide the bytes per second by this value before outputting.

Example:

```
./bps -i ens18 [-c <"kbps", "mbps", or "gbps"> --custom <integer>]
```

## Credits
* [Christian Deacon](https://github.com/gamemann)