#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;


out vec3 TexCoords;

uniform mat4 u_ViewProjMat;

void main()
{
	TexCoords = a_Position;
	vec4 pos = u_ViewProjMat * vec4(a_Position, 1.0);
	gl_Position = pos.xyzw;
}

#type fragment
#version 450 core
out vec4 FragColor;

in vec3 TexCoords;
uniform samplerCube u_Skybox;

void main()
{
	FragColor = texture(u_Skybox, TexCoords);
}