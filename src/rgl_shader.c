#include "rgl_shader.h"
#include "rgl_log.h"

rgl_shader_t rgl_sprite_shader;

void rgl_shader_initialize(rgl_shader_t *shader, const char *vert_src, const char *frag_src) {
	id_t vert, frag;
	int success;
	char infolog[512];

	vert = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vert, 1, &vert_src, NULL);
	glCompileShader(vert);
	glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vert, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to compile vertex shader: %s", infolog);
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &frag_src, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to compile fragment shader: %s", infolog);
	}

	shader->id = glCreateProgram();
	glAttachShader(shader->id, vert);
	glAttachShader(shader->id, frag);
	glLinkProgram(shader->id);

	glGetProgramiv(shader->id, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader->id, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to link shader: %s", infolog);
	}

	glValidateProgram(shader->id);
	glGetProgramiv(shader->id, GL_VALIDATE_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(shader->id, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to validate shader: %s", infolog);
	}


	glDeleteShader(vert);
	glDeleteShader(frag);
}

void rgl_shader_create_primitives() {
	rgl_shader_initialize(&rgl_sprite_shader, 
		// VERTEX SHADER
		"#version 330 core\n" \
		"layout (location = 0) in vec2 aPos;\n" \
		"layout (location = 1) in vec2 aUV;\n" \
		"uniform mat4 transform;\n" \
		"out vec2 UV;\n" \
		"void main() {\n" \
		"\tgl_Position = transform * vec4(aPos, 0.0, 1.0);\n" \
		"\tUV = aUV;\n" \
		"}\n",

		// FRAGMENT SHADER
		"#version 330 core\n" \
		"out vec4 FragColor;\n" \
		"in vec2 UV;\n" \
		"uniform sampler2D Texture;\n" \
		"void main() {\n" \
		"\tFragColor = texture(Texture, UV);\n" \
		"}\n"
	);
}
