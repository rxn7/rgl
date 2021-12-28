SRC = -c src/*.c
INCS = -Iinclude -Isrc
CFLAGS = -std=gnu99 -Wall -fPIC -g
OUT = -o out/librgl.so
LIBS = $(shell pkg-config --static --libs glfw3)
OBJ = glad.o \
      rgl.o \
      rgl_texture.o \
      rgl_file.o \
      rgl_input.o 

all: compile link clean
	
compile:
	gcc $(SRC) $(INCS) $(CFLAGS)

link:
	gcc -shared $(OBJ) $(LIBS) $(OUT)

clean:
	rm -rf *.o

install:
	make
	sudo mkdir -p /usr/include/rgl
	sudo cp -r include/* /usr/include/rgl/
	sudo cp out/* /usr/lib/
