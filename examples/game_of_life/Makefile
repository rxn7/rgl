CFLAGS := -std=gnu99
SRC := ./src/*.c
OBJ := game_of_life.o

all: compile link

compile:
	gcc -c $(SRC) -I./src $(CFLAGS)

link:
	gcc $(OBJ) -lrgl -o game_of_life
