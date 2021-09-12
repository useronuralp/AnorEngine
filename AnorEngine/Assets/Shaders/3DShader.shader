#type vertex
#version 450 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec2 a_Uv; 
layout (location = 2) in vec3 a_Normal;

uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform mat4 u_DirectionalLightViewProjMatrix;

out vec3 v_Normal;
out vec2 v_UV;
out vec3 v_Position;
out vec4 v_FragPosLightSpace;

void main()
{	
	v_UV = a_Uv;
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0f));
	gl_Position = u_ViewProjMat * vec4(v_Position, 1.0f);
	v_FragPosLightSpace = (u_DirectionalLightViewProjMatrix * vec4(v_Position, 1.0));
}

#type fragment
#version 450 core
#define MAX_POINT_LIGHT_NUMBER 10

layout(location = 0) out vec4 Color;
layout(location = 1) out int IntegerColorBuffer;


//Custom variables
struct Material {
	float ambientIntensity;
	float diffuseIntensity;
	float specularIntensity;
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
	float castShadow;
};

in vec3 v_Normal;
in vec2 v_UV;
in vec3 v_Position;
in vec4 v_FragPosLightSpace;

uniform vec3		u_CameraPos;
uniform vec4		u_Color;
uniform samplerCube u_Skybox;
uniform sampler2D	u_Sampler;
uniform vec3		u_DirectionalLightPosition;
uniform vec3		u_DirectionalLightColor;
uniform sampler2D   u_DirectionalShadowMap;
uniform float		u_CastDirectionalLight;
uniform PointLight	u_PointLights[MAX_POINT_LIGHT_NUMBER];
uniform samplerCube u_PointLightShadowMap[MAX_POINT_LIGHT_NUMBER];
uniform int			u_PointLightCount;
uniform float		u_PointLigthFarPlane;
uniform int		    u_EntityID;

uniform sampler2D   u_TextureSamplerDiffuse;
uniform sampler2D   u_TextureSamplerSpecular;

vec3 directionalLight = vec3(0.0f, 0.0f, -1.0f);

float PointShadowCalculation(vec3 fragPos, int index)
{
	float shadow;
	//If the fragment is lit by a direct light, conclude that it is not occluded by shadow.
	vec3 lightToFrag = fragPos - u_PointLights[index].position;
	float depth = texture(u_PointLightShadowMap[index], lightToFrag).r;
	depth *= u_PointLigthFarPlane;
	float bias = 0.5;
	shadow = (depth + bias) < length(lightToFrag) ? 0.0 : 1.0f;
	if (shadow == 1.0f)
		return 1.0f;
	//--------------end of direct light check

	for (int i = 0; i < u_PointLightCount; i++)
	{
		lightToFrag = fragPos - u_PointLights[i].position;
		depth = texture(u_PointLightShadowMap[i], lightToFrag).r;
		depth *= u_PointLigthFarPlane;
		bias = 0.5;
		shadow = (depth + bias) < length(lightToFrag) ? 0.0 : 1.0f;
		//break if the fragment is occluded by any of the point lights in the scene, no need for further if checks.
		if (shadow == 0.0f)
			return 0.0f;
	}
	return shadow;
}

float DirectionalShadowCalculation(vec4 fragPosLightSpace, vec3 fragPos, vec3 lightPosition, vec3 normal)
{
	// perform perspective divide
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// transform to [0,1] range
	projCoords = projCoords * 0.5 + 0.5;
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	float closestDepth = texture(u_DirectionalShadowMap, projCoords.xy).r;
	// get depth of current fragment from light's perspective
	float currentDepth = projCoords.z;
	//Shadow bias to eliminate shadow acne, this bias might need tweaking in the future for each scene.
	vec3 lightDir = normalize(lightPosition - fragPos);
	float bias = max(0.0001 * (1.0 - dot(normal, lightDir)), 0.000001);
	// check whether current frag pos is in shadow
	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(u_DirectionalShadowMap, 0);
	for (int x = -1; x <= 1; ++x)
	{
		//Sampling from the same depth mat multiple times and averaging the shadows to have smooth shadows.
		for (int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture(u_DirectionalShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;
	if (projCoords.z > 1.0)
		shadow = 0.0;
	return shadow;
}


vec3 CalcDirectionalLight(vec3 Normal, vec3 viewDir, vec3 FragPosition, float shadow, vec4 specularTex)
{
	vec3 DirectionalLightDir = normalize(u_DirectionalLightPosition - FragPosition);
	//Ambient
	vec3 AmbientDirectional = vec3(0.1f) * u_DirectionalLightColor;
	//Diffuse
	vec3 DiffuseDirectional = max(dot(normalize(Normal), DirectionalLightDir), 0.0) * u_DirectionalLightColor * vec3(1.0f);
	//Specular
	vec3 SpecularDirectional = (vec3(u_DirectionalLightColor) * pow(max(dot(viewDir, reflect(-DirectionalLightDir, normalize(Normal))), 0.0), 100.0f)) * vec3(specularTex) * vec3(1.0f);

	return ((1.0 - shadow) * (DiffuseDirectional + SpecularDirectional) + AmbientDirectional);
}

//Returns only the required intensity of the point light on the object. You need to multiply the texture or any other colors values seperately with the result of this function.
vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPosition, vec3 viewDir, vec4 specularTex, float shadow)
{
	vec3 lightDir = normalize(light.position - FragPosition);
	//Used for Blinn-Phong lighting model.
	vec3 halfwayDir = normalize(lightDir + viewDir);

	// ambient
	vec3 ambient = vec3(light.color) * vec3(0.1f);

	// diffuse 
	vec3 norm = normalize(Normal);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(light.diffuse) * vec3(light.color) * diff;

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), 100.0f);
	vec3 specular = vec3(light.specular) * vec3(light.color) * spec * vec3(specularTex);

	// attenuation
	float distance = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.constant + light.Linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (((shadow) * (diffuse + specular)) + (ambient * 0.0f)) * light.intensity;
}

void main()
{
	float distance = length(vec3(2.0f, 2.0f, 0.0f) - v_Position);

	vec4 diffuseTexColor = texture(u_TextureSamplerDiffuse, v_UV);
	vec4 specularTexColor = texture(u_TextureSamplerSpecular, v_UV);


	vec3 viewDir = normalize(u_CameraPos - v_Position);
	vec3 FinalColor = { 0,0,0 };

	vec3 ObjectProperties = vec3(diffuseTexColor) * vec3(u_Color);

	float directionalShadow = DirectionalShadowCalculation(v_FragPosLightSpace, v_Position, u_DirectionalLightPosition, v_Normal);
	if (u_CastDirectionalLight > 0.5f)
		FinalColor += CalcDirectionalLight(v_Normal, viewDir, v_Position, directionalShadow, specularTexColor) * ObjectProperties ;


	for (int i = 0; i < u_PointLightCount; i++)
	{
		//Point Light shadow
		float pointShadow = PointShadowCalculation(v_Position, i);
		if (u_PointLights[i].castShadow > 0.5f)
			FinalColor += CalcPointLight(u_PointLights[i], v_Normal, v_Position, viewDir, specularTexColor, pointShadow) * ObjectProperties;
		else
			FinalColor += CalcPointLight(u_PointLights[i], v_Normal, v_Position, viewDir, specularTexColor, 1) * ObjectProperties;
	}

	//FinalColor = vec3(1.0f);
	Color = vec4(FinalColor, 1.0);
	IntegerColorBuffer = u_EntityID;
}

