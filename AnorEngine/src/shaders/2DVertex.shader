#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;


uniform mat4 u_viewprojMat;

out vec3 m_Position;
out vec4 m_Color;
void main()
{
	gl_Position = (u_viewprojMat) * vec4(position, 1.0f);
	m_Color = color;
}

