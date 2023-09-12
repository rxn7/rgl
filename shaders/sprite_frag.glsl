#version 330 core

in vec2 UV;
out vec4 FragColor;

uniform sampler2D u_Texture;

void main() {
	FragColor = texture(u_Texture, UV);
}
