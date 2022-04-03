SRC := ./src/*.cc
OBJ := balls.o ball.o

all: compile link

compile:
	g++ -c $(SRC) -I./src $(CFLAGS)

link:
	g++ $(OBJ) -lrgl -o balls
