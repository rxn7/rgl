#include "rgl_shader.h"
#include "rgl_log.h"

rgl_shader_t *g_rgl_sprite_shader;

b8 rgl_shader_create(rgl_shader_t *shader, const char *vert_src, const char *frag_src) {
	if(!shader) {
		RGL_LOG_ERROR("Can't create shader with null pointer");
		return false;
	}

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
		return false;
	}

	frag = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag, 1, &frag_src, NULL);
	glCompileShader(frag);
	glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(frag, 512, NULL, infolog);
		RGL_LOG_ERROR("Failed to compile fragment shader: %s", infolog);
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

b8 rgl_shader_destroy(rgl_shader_t *shader) {
	if(!shader) {
		RGL_LOG_ERROR("Can't destroy shader with null pointer");
		return false;
	}

	glDeleteProgram(shader->id);

	if(shader->uniform_locations) {
		free(shader->uniform_locations);
	}

	return true;
}

void rgl_shader_create_defaults() {
	g_rgl_sprite_shader = malloc(sizeof(rgl_shader_t));

	/* Sprite shader */
	rgl_shader_create(g_rgl_sprite_shader, 
		/* VERTEX SHADER */
		"#version 330 core\n" \
		"layout (location = 0) in vec2 a_Pos;\n" \
		"layout (location = 1) in vec2 a_UV;\n" \
		"uniform mat4 u_Projection;\n" \
		"uniform mat4 u_Model;\n" \
		"out vec2 UV;\n" \
		"void main() {\n" \
		"	gl_Position = u_Projection * u_Model * vec4(a_Pos, 0.0, 1.0);\n" \
		"	UV = a_UV;\n" \
		"}\n",

		/* FRAGMENT SHADER */
		"#version 330 core\n" \
		"out vec4 FragColor;\n" \
		"in vec2 UV;\n" \
		"uniform sampler2D u_Texture;\n" \
		"void main() {\n" \
		"	FragColor = texture(u_Texture, UV);\n" \
		"}\n"
	);

	g_rgl_sprite_shader->uniform_locations = calloc(2, sizeof(u32));
	g_rgl_sprite_shader->uniform_locations[0] = glGetUniformLocation(g_rgl_sprite_shader->id, "u_Projection");
	g_rgl_sprite_shader->uniform_locations[1] = glGetUniformLocation(g_rgl_sprite_shader->id, "u_Model");
}

void rgl_shader_destroy_defaults() {
	rgl_shader_destroy(g_rgl_sprite_shader);
	free(g_rgl_sprite_shader);
}
