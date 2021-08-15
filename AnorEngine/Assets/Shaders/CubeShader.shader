#type vertex
#version 450 core

//Need position values here!!!
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 aNormal;

uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform int u_EntityID;

out vec3 Normal;
out vec3 Position;
out flat int v_EntityID;

void main()
{
	Normal = mat3(transpose(inverse(u_Transform))) * aNormal;
	Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = (u_ViewProjMat * u_Transform) * vec4(a_Position, 1.0);
	v_EntityID = u_EntityID;
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color2;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform vec4 u_Color;

in flat int v_EntityID;
in vec3 Normal;
in vec3 Position;
void main()
{
	vec3 I = normalize(Position - cameraPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
	//FragColor = u_Color;
	color2 = v_EntityID;
}