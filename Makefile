CC = clang

COMMOBJS += src/cmdline.o src/common.o

STATSRC += src/stat.c

all: stat

stat: $(COMMOBJS)
	clang -o gstat $(COMMOBJS) $(STATSRC)

install:
	cp gstat /usr/bin/

clean:
	rm -f src/*.o
	rm -f gstat

.PHONY: stat
.DEFAULT: all
