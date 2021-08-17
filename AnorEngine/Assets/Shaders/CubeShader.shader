#type vertex
#version 450 core

//Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

//Custom variables


//Varying OUTSs
out vec3 Normal;
out vec3 Position;
out flat int v_EntityID;
out vec2 v_UV;

//Uniforms
uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform int u_EntityID;

void main()
{
	v_UV = a_UV;
	Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = (u_ViewProjMat * u_Transform) * vec4(a_Position, 1.0);
	v_EntityID = u_EntityID;
}

#type fragment
#version 450 core
//There is a limit to this define number in glsl.
#define MAX_POINT_LIGHT_NUMBER 100 


//Attributes
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int color2;

//Custom variables
struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	float metalness;
};

struct PointLight {

	float constant;
	float Linear;
	float quadratic;

	float intensity;

	vec4 color;
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


//Varying INs
in flat int v_EntityID;
in vec3 Normal;
in vec3 Position;
in vec2 v_UV;

//Uniforms
uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform vec4 u_Color;
uniform PointLight pointLights[MAX_POINT_LIGHT_NUMBER];
uniform int pointLightCount;
uniform Material material;
uniform vec3 directionalLightColor;
uniform vec3 directionalLight;
uniform sampler2D u_Sampler;

vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPosition, vec3 viewDir)
{

	// ambient    //intensity
	vec3 ambient = light.ambient * vec3(light.color) * material.ambient;

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * vec3(light.color) * diff * material.diffuse;

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * vec3(light.color) * spec * material.specular;


	// attenuation
	float distance = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.constant + light.Linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular) * light.intensity;
}


void main()
{

	//Refraction
	float ratio = 1.00 / 1.52;
	vec3 Irefract = normalize(Position - cameraPos);
	vec3 Rrefract = refract(Irefract, normalize(Normal), ratio);
	vec4 refraction = vec4(texture(skybox, Rrefract).rgb, 1.0);

	//Reflection
	vec3 Ireflect = normalize(Position - cameraPos);
	vec3 Rreflect = reflect(Ireflect, normalize(Normal));
	vec4 reflection = vec4(texture(skybox, Rreflect).rgb, 1.0);

	vec3 viewDir = normalize(cameraPos - Position);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	vec4 texColor = texture(u_Sampler, v_UV);
	for (int i = 0; i < pointLightCount; i++)
		result += CalcPointLight(pointLights[i], Normal, Position, viewDir) * vec3(texColor) * vec3(u_Color) + (vec3(reflection) * material.metalness);

	
	FragColor = vec4(result, 1.0);



	color2 = v_EntityID;
}