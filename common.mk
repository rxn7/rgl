SRC = -c src/*.c
INCS = -Iinclude -Isrc
CFLAGS = -std=gnu17 -Wall -fPIC
LIBS = -lepoxy -lm -lopenal -lsndfile

OBJ = rgl.o \
      rgl_texture.o \
      rgl_file.o \
      rgl_input.o \
      rgl_vao.o \
      rgl_audio.o \
      rgl_sprite.o \
      rgl_shader.o \
      rgl_immediate.o \
      rgl_v2.o \
      rgl_mat4.o \

all: compile link clean
