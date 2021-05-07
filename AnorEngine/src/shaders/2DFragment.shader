#version 330 core

layout(location = 0) out vec4 color;

in vec3 m_Position;
in vec4 m_Color;

void main()
{
	color = m_Color;
}