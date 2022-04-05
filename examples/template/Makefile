CFLAGS := -std=gnu99
SRC := ./src/*.c
OBJ := main.o

all: compile link

compile:
	gcc -c $(SRC) -I./src $(CFLAGS)

link:
	gcc $(OBJ) -lrgl -o template
