CC = clang

COMMOBJS += src/cmdline.o src/common.o

STATSRC += src/stat.c

all: stat

stat: $(COMMOBJS)
	clang -o stat $(COMMOBJS) $(STATSRC)

install:
	cp stat /usr/bin/

clean:
	rm -f src/*.o
	rm -f stat

.PHONY: stat
.DEFAULT: all