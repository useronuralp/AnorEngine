#type vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 texcoord;

uniform mat4 u_ViewProjMat;
uniform mat4 u_ModelMatrix;

out vec4 v_Color;
out vec3 v_TexCoord;

void main()
{
	gl_Position = (u_ViewProjMat) * vec4(position, 1.0f);
	v_TexCoord = texcoord;
	v_Color = color;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 m_Position;
in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;

void main()
{
	color = v_Color;
}