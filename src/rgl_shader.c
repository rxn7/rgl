#include "rgl_shader.h"
#include "rgl_sprite.h"
#include "rgl_log.h"

static b8 _init_shader(u32 *shader, u32 type, const char *src) {
	s32 success;
	char infolog[512];

	u32 id = glCreateShader(type);
	glShaderSource(id, 1, &src, 0);
	glCompileShader(id);

	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(id, 512, 0, infolog);
		RGL_LOG_ERROR("Failed to compile %s shader: %s", type == GL_VERTEX_SHADER ? "vertex" : "fragment", infolog);
		return false;
	}

	*shader = id;

	return true;
}

b8 rgl_shader_create(rgl_shader_t *shader, const char *vert_src, const char *frag_src) {
#ifdef RGL_DEBUG
	if(!shader) {
		RGL_LOG_ERROR("Can't create shader with null pointer");
		return false;
	}
#endif

	id_t vert, frag;
	int success;
	char infolog[512];

	_init_shader(&vert, GL_VERTEX_SHADER, vert_src);
	_init_shader(&frag, GL_FRAGMENT_SHADER, frag_src);

	shader->id = glCreateProgram();
	glAttachShader(shader->id, vert);
	glAttachShader(shader->id, frag);

	glLinkProgram(shader->id);
	glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader->id, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to link shader: %s", infolog);
		return false;
	}

	glValidateProgram(shader->id);
	glGetProgramiv(shader->id, GL_VALIDATE_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader->id, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to validate shader: %s", infolog);
		return false;
	}

	glDeleteShader(vert);
	glDeleteShader(frag);

	return true;
}

b8 rgl_shader_destroy(rgl_shader_t *shader) {
#ifdef RGL_DEBUG
	if(!shader) {
		RGL_LOG_ERROR("Can't destroy shader with null pointer");
		return false;
	}
#endif

	glDeleteProgram(shader->id);

	if(shader->uniform_locations) {
		free(shader->uniform_locations);
	}

	return true;
}

void rgl_shader_create_defaults() {
	rgl_sprite_shader_create();
}

void rgl_shader_destroy_defaults() {
	rgl_sprite_shader_destroy();
}
