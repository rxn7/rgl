OUT := out/librgl.so
LIBS := $(shell pkg-config --static --libs x11) $(shell pkg-config --static --libs glx)
DIR_SRC := src/x11
CC := gcc

include common.mk

install:
	make -f linux.mk
	sudo mkdir -p /usr/include/rgl
	sudo cp -r include/* /usr/include/rgl/
	sudo cp $(OUT) /usr/lib/
