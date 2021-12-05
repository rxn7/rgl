SRC = -c src/*.c
INCS = -Irgl
CFLAGS = -std=gnu99
LIB = -lglfw3
OUT = out/librgl.so
OBJ = rgl.o

all: compile link clean
	
compile:
	gcc $(SRC) $(INCS) $(CFLAGS)

link:
	ar -rc $(OUT) $(OBJ) $(LIB)

clean:
	rm -rf *.o
