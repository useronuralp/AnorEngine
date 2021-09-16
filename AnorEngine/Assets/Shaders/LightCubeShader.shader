#type vertex
#version 450 core

//Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

//OUTs
out flat int v_EntityID;
out vec2 v_UV;

//Uniforms
uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform int u_EntityID;

void main()
{
	v_UV = a_UV;
	gl_Position = (u_ViewProjMat * u_Transform) * vec4(a_Position, 1.0);
	v_EntityID = u_EntityID;
}

#type fragment
#version 450 core
//Attributes
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color2;

in flat int v_EntityID;
in vec2 v_UV;
//Uniforms
uniform vec4 u_Color;
uniform sampler2D u_TextureSamplerDiffuse;

void main()
{
						//Rocking a plain white texture atm.
	FragColor = u_Color * texture(u_TextureSamplerDiffuse, v_UV);
	color2 = v_EntityID;
}