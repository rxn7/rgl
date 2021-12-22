SRC = -c src/*.c
INCS = -I.
CFLAGS = -std=gnu99 -Wall -fPIC
OUT = -o out/librgl.so
OBJ = glad.o rgl.o rgl_texture.o
LIBS = $(shell pkg-config --static --libs glfw3)
all: compile link
	
compile:
	gcc $(SRC) $(INCS) $(CFLAGS)

link:
	gcc -shared $(OBJ) $(LIBS) $(OUT)

clean:
	rm -rf *.o

install:
	make
	sudo cp -r rgl /usr/include/
	sudo cp out/* /usr/lib/
