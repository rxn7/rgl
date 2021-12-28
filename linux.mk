include common.mk
OUT = -o out/librgl.so
LIBS = $(shell pkg-config --static --libs glfw3)

all: compile link clean
	
compile:
	gcc $(SRC) $(INCS) $(CFLAGS)

link:
	gcc -shared $(OBJ) $(LIBS) $(OUT)

clean:
	rm -rf *.o
