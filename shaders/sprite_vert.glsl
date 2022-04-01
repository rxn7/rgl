#version 330 core
layout (location = 0) in vec2 a_Pos;
layout (location = 1) in vec2 a_UV;

uniform mat4 u_Projection;
uniform mat4 u_Model;

out vec2 UV;

void main() {
	gl_Position = u_Projection * u_Model * vec4(a_Pos, 0.0, 1.0);
	UV = a_UV;
}
