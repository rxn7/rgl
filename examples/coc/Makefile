CFLAGS := -std=gnu99 -g
SRC := ./src/*.c
OBJ := coc.o player.o coin.o

all: compile link

compile:
	gcc -c $(SRC) -I./src $(CFLAGS)

link:
	gcc $(OBJ) -lrgl -o coc
