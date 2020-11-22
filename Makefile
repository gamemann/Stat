CC = clang

ppsobjs += src/common.o src/pps.o
bpsobjs += src/common.o src/bps.o

all: pps bps

pps: $(ppsobjs)
	clang -o pps $(ppsobjs)

bps: $(bpsobjs)
	clang -o bps $(bpsobjs)

clean:
	rm -f src/*.o
	rm -f pps
	rm -f bps

.PHONY: pps bps

.DEFAULT: all