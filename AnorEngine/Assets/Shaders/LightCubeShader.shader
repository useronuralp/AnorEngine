#type vertex
#version 450 core

//Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;

//Custom variables


//Varying OUTSs
out vec3 Normal;
out vec3 Position;
out flat int v_EntityID;

//Uniforms
uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform int u_EntityID;

void main()
{
	Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = (u_ViewProjMat * u_Transform) * vec4(a_Position, 1.0);
	v_EntityID = u_EntityID;
}

#type fragment
#version 450 core


//Attributes
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color2;

//Custom variables
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct LightIntensity {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


//Varying INs
in flat int v_EntityID;
in vec3 Normal;
in vec3 Position;

//Uniforms
uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform vec4 u_Color;
uniform LightIntensity lightIntensity;
uniform Material material;
uniform vec3 pointLightPos;
uniform vec4 pointLightColor;
uniform vec3 directionalLightColor;
uniform vec3 directionalLight;


void main()
{
	FragColor = u_Color;
	color2 = v_EntityID;
}