SRC = -c src/*.c
INCS = -Iinclude -Isrc
CFLAGS = -std=gnu99 -Wall -fPIC -DRGL_BUILD
LIBS = -lepoxy -lm -lopenal

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
      rgl_assert.o \
      rgl_sprite_animation.o \
      rgl_sprite_animator.o \
      rgl_animation_texture.o \
      rgl_camera.o

all: compile link
