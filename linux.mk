include common.mk
OUT = -o out/librgl.so
LIBS = $(shell pkg-config --static --libs x11) $(shell pkg-config --static --libs glx)

SRC := $(SRC) -c src/x11/*.c
OBJ := $(OBJ) rgl_x11_context.o rgl_x11_input.o

all: compile link clean
	
compile:
	gcc $(SRC) $(INCS) $(CFLAGS)

link:
	gcc -shared $(OBJ) $(LIBS) $(OUT)

clean:
	rm -rf *.o
