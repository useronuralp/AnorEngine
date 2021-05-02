#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 uv; 
layout(location = 2) in vec4 color;


uniform mat4 pr_matrix;
uniform mat4 vw_matrix = mat4(1.0);
uniform mat4 ml_matrix = mat4(1.0);

out vec4 o_Color;
out vec2 o_Tex;

void main()
{
	gl_Position = position;
	o_Color = color;
	o_Tex = uv;
}

