// Vertex Shader � file "particles.vert"

#version 330

layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Color;
out vec4 ex_Color;


uniform mat4 modelview;
uniform mat4 projection;

void main(void)
{
	ex_Color = vec4(in_Color, 1.0f);
	
	gl_Position = projection *  modelview * vec4(in_Position, 1.0);
}