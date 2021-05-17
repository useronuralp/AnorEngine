#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMatrix;
out vec4 m_Color;
out vec2 m_texCoord;
void main()
{
	gl_Position = (u_ViewProjMat) * (u_ModelMatrix)*vec4(position, 1.0f);
	m_texCoord = texCoord;
}