CC = clang

ppsobjs += src/pps.o
bpsobjs += src/bps.o

commobjs += src/cmdline.o src/common.o

all: pps bps

pps: $(commobjs) $(ppsobjs)
	clang -o pps $(commobjs) $(ppsobjs)

bps: $(commobjs) $(bpsobjs)
	clang -o bps $(commobjs) $(bpsobjs)

clean:
	rm -f src/*.o
	rm -f pps
	rm -f bps

.PHONY: pps bps

.DEFAULT: all