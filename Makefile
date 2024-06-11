CC = clang

BUILD_DIR = build
SRC_DIR = src

COMM_OBJS += $(BUILD_DIR)/cmdline.o $(BUILD_DIR)/common.o

all: common stat

common:
	clang -O2 -o $(BUILD_DIR)/cmdline.o -c $(SRC_DIR)/cmdline.c
	clang -O2 -o $(BUILD_DIR)/common.o -c $(SRC_DIR)/common.c

stat: $(COMM_OBJS)
	clang -O2 -o $(BUILD_DIR)/gstat $(COMM_OBJS) $(SRC_DIR)/stat.c

install:
	cp $(BUILD_DIR)/gstat /usr/bin/

clean:
	rm -f $(SRC_DIR)/*.o
	rm -f $(BUILD_DIR)/gstat

.PHONY: all
.DEFAULT: all
