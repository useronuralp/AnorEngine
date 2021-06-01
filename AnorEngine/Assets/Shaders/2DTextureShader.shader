#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMatrix;
out vec4 m_Color;
out vec2 m_texCoord;
void main()
{
	gl_Position = (u_ViewProjMat) * (u_ModelMatrix) * vec4(position, 1.0f);
	m_texCoord = texCoord;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 m_Position;
in vec4 m_Color;
in vec2 m_texCoord;
uniform vec4 u_Color;
uniform sampler2D u_Sampler;

void main()
{
	color = texture(u_Sampler, m_texCoord);
	//color = vec4(m_texCoord, 1.0f, 1.0f);
}