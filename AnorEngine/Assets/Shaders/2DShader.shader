#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMatrix;
out vec4 m_Color;
void main()
{
	gl_Position = (u_ViewProjMat) * (u_ModelMatrix)*vec4(position, 1.0f);
	m_Color = color;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 m_Position;
in vec4 m_Color;
uniform vec4 u_Color;

void main()
{
	color = u_Color;
}