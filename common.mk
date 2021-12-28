SRC = -c src/*.c
INCS = -Iinclude -Isrc
CFLAGS = -std=gnu99 -Wall -fPIC

OBJ = glad.o \
      rgl.o \
      rgl_texture.o \
      rgl_file.o \
      rgl_input.o 
