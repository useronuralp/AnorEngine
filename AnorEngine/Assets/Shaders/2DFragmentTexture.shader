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
}