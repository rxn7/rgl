SRC = -c src/*.c
INCS = -Iinclude -Isrc
CFLAGS = -std=gnu17 -Wall -fPIC -O2 
LIBS = -lepoxy -lm

OBJ = rgl.o \
      rgl_texture.o \
      rgl_file.o \
      rgl_input.o \
      rgl_vbo.o \
      rgl_sprite.o \
      rgl_shader.o \
      rgl_transform.o \
      rgl_immediate.o
