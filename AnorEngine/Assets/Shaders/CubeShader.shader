#type vertex
#version 450 core

//Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

//Custom variables


//Varying OUTSs
out vec3 v_Normal;
out vec3 v_Position;
out flat int v_EntityID;
out vec2 v_UV;

//Uniforms
uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform int u_EntityID;

void main()
{
	v_UV = a_UV;
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = (u_ViewProjMat * u_Transform) * vec4(a_Position, 1.0);
	v_EntityID = u_EntityID;
}

#type fragment
#version 450 core
//There is a limit to this define number in glsl.
#define MAX_POINT_LIGHT_NUMBER 100 


//Attributes
layout(location = 0) out vec4 FragColor;
layout(location = 1) out int IntegerColorBuffer;

//Custom variables
struct Material {
	float ambient;
	float diffuse;
	float specular;
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
	float ambient;
	float diffuse;
	float specular;
};


//Varying INs
in flat int v_EntityID;
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_UV;

//Uniforms
uniform vec3 u_CameraPos;
uniform samplerCube u_Skybox;
uniform vec4 u_Color;
uniform PointLight u_PointLights[MAX_POINT_LIGHT_NUMBER];
uniform int u_PointLightCount;
uniform Material u_Material;
uniform vec3 u_DirectionalLightColor;
uniform vec3 u_DirectionalLight;
uniform sampler2D u_Sampler;

vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPosition, vec3 viewDir)
{

	// ambient    //intensity
	vec3 ambient = vec3(light.ambient) * vec3(light.color) * vec3(u_Material.ambient);

	// diffuse 
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(light.diffuse) * vec3(light.color) * diff * vec3(u_Material.diffuse);

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);
	vec3 specular = vec3(light.specular) * vec3(light.color) * spec * vec3(u_Material.specular);


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
	vec3 Irefract = normalize(v_Position - u_CameraPos);
	vec3 Rrefract = refract(Irefract, normalize(v_Normal), ratio);
	vec4 refraction = vec4(texture(u_Skybox, Rrefract).rgb, 1.0);

	//Reflection
	vec3 Ireflect = normalize(v_Position - u_CameraPos);
	vec3 Rreflect = reflect(Ireflect, normalize(v_Normal));
	vec4 reflection = vec4(texture(u_Skybox, Rreflect).rgb, 1.0);

	vec3 viewDir = normalize(u_CameraPos - v_Position);
	vec3 result = vec3(0.0f, 0.0f, 0.0f);

	vec4 texColor = texture(u_Sampler, v_UV);
	for (int i = 0; i < u_PointLightCount; i++)
		result += CalcPointLight(u_PointLights[i], v_Normal, v_Position, viewDir) * vec3(texColor) * vec3(u_Color) + (vec3(reflection) * u_Material.metalness);

	
	FragColor = vec4(result, 1.0);
	IntegerColorBuffer = v_EntityID;
}