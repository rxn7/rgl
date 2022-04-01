"#version 330 core\n"
"in vec2 UV;\n"
"out vec4 FragColor;\n"

"uniform sampler2D u_Texture;\n"

"void main() {\n"
"	FragColor = texture(u_Texture, UV);\n"
"}"
