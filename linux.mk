include common.mk
OUT = out/librgl.so
LIBS := $(LIBS) $(shell pkg-config --static --libs x11) $(shell pkg-config --static --libs glx)

SRC := $(SRC) -c src/x11/*.c
OBJ := $(OBJ) rgl_x11_context.o rgl_x11_input.o
	
compile:
	gcc $(SRC) $(INCS) $(CFLAGS)

link:
	gcc -shared $(OBJ) $(LIBS) -o $(OUT)

clean:
	rm -rf *.o

install:
	mkdir -p out
	make -f linux.mk
	sudo mkdir -p /usr/include/rgl
	sudo cp -r include/* /usr/include/rgl/
	sudo cp $(OUT) /usr/lib/
