.PHONY: all install

CC := gcc
OUT := out/librgl.so
DIR_SRC += src/x11
LIBS += $(shell pkg-config --static --libs x11) $(shell pkg-config --static --libs glx)

include make/common.mk

all: compile

install:
	sudo mkdir -p /usr/include/rgl
	sudo cp -r include/* /usr/include/rgl/
	sudo cp $(OUT) /usr/lib/
