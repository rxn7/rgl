"#version 330 core\n"
"layout (location = 0) in vec2 a_Pos;\n"
"layout (location = 1) in vec2 a_UV;\n"

"uniform mat4 u_Projection;\n"
"uniform mat4 u_Model;\n"

"out vec2 UV;\n"

"void main() {\n"
"	gl_Position = u_Projection * u_Model * vec4(a_Pos, 0.0, 1.0);\n"
"	UV = a_UV;\n"
"}"
