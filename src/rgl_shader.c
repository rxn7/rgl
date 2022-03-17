#include "rgl_shader.h"
#include "rgl_sprite.h"
#include "rgl_log.h"

b8 _rglShaderInit(u32 *shader, u32 type, const char *src) {
	RGL_ASSERT_VALID_PTR(shader);

	i32 success;
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

b8 rglShaderCreate(rglShader *shader, const char *vert_src, const char *frag_src) {
	RGL_ASSERT_VALID_PTR(shader);

	id_t vert, frag;
	int success;
	char infolog[512];

	if(!_rglShaderInit(&vert, GL_VERTEX_SHADER, vert_src)) {
		return false;
	}

	if(!_rglShaderInit(&frag, GL_FRAGMENT_SHADER, frag_src)) {
		return false;
	}

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

void rglShaderDestroy(rglShader *shader) {
	RGL_ASSERT_VALID_PTR(shader);

	glDeleteProgram(shader->id);

	if(shader->uniform_locations) {
		free(shader->uniform_locations);
	}
}

void _rglShaderCreateDefaults(void) {
	_rglSpriteShaderCreate();
}

void _rglShaderDestroyDefaults(void) {
	_rglSpriteShaderDestroy();
}
