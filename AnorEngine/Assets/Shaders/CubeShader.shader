#type vertex
#version 450 core

//Attributes
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_UV;

//Varying OUTSs
out vec3 v_Normal;
out vec3 v_Position;
out vec2 v_UV;
out vec4 v_FragPosLightSpace;

//Uniforms
uniform mat4 u_Transform = mat4(1.0);
uniform mat4 u_ViewProjMat = mat4(1.0);
uniform mat4 u_DirectionalLightViewProjMatrix;

void main()
{
	v_UV = a_UV;
	v_Normal = mat3(transpose(inverse(u_Transform))) * a_Normal;
	v_Position = vec3(u_Transform * vec4(a_Position, 1.0));
	gl_Position = u_ViewProjMat * vec4(v_Position, 1.0);
	v_FragPosLightSpace = (u_DirectionalLightViewProjMatrix * vec4(v_Position, 1.0));
}

#type fragment
#version 450 core
//There is a limit to this define number in glsl.
#define MAX_POINT_LIGHT_NUMBER 10


//Attributes
layout(location = 0) out vec4 FragColor;
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

//Varying INs
in vec3 v_Normal;
in vec3 v_Position;
in vec2 v_UV;
in vec4 v_FragPosLightSpace;


//Uniforms
uniform vec3		u_CameraPos;
uniform vec4		u_Color;
uniform Material	u_Material;
uniform samplerCube u_Skybox;
uniform vec3		u_DirectionalLightPosition;
uniform vec3		u_DirectionalLightColor;
uniform sampler2D   u_DirectionalShadowMap;
uniform float		u_CastDirectionalLight;
uniform PointLight	u_PointLights[MAX_POINT_LIGHT_NUMBER];
uniform samplerCube u_PointLightShadowMap[MAX_POINT_LIGHT_NUMBER];
uniform int			u_PointLightCount;
uniform float		u_PointLigthFarPlane;
uniform int		    u_EntityID;

uniform sampler2D   u_DiffuseMap;

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

//Returns only the required intensity of the point light on the object. You need to multiply the texture or any other colors values seperately with the result of this function.
vec3 CalcPointLight(PointLight light, vec3 Normal, vec3 FragPosition, vec3 viewDir, float shadow)
{
	vec3 lightDir = normalize(light.position - FragPosition);
	vec3 norm = normalize(Normal);

	// ambient
	vec3 ambient = vec3(light.color) * vec3(u_Material.ambientIntensity);

	// diffuse 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = vec3(light.diffuse) * vec3(light.color) * diff * vec3(u_Material.diffuseIntensity);

	// specular
	//Halfway direction is for Blinn-Phong lighting model.
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Material.shininess);
	vec3 specular = vec3(light.specular) * vec3(light.color) * spec * vec3(u_Material.specularIntensity);

	// attenuation
	float distance = length(light.position - FragPosition);
	float attenuation = 1.0 / (light.constant + light.Linear * distance + light.quadratic * (distance * distance));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	//Currently not adding the ambient component to the calculation.
	//return (((ambient * 0.0f) + diffuse + specular) * light.intensity);

	return (((shadow) * (diffuse + specular)) + (ambient * 0.0f)) * light.intensity;
}

//Returns only the required intensity of the directional light on the object. You need to multiply the texture or any other colors values seperately with the result of this function.
vec3 CalcDirectionalLight(vec3 Normal, vec3 viewDir,  vec3 FragPosition, float shadow)
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(u_DirectionalLightPosition - FragPosition);
	//Ambient
	vec3 AmbientDirectional = vec3(u_Material.ambientIntensity) * u_DirectionalLightColor;
	//Diffuse
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 DiffuseDirectional = diff * u_DirectionalLightColor * vec3(u_Material.diffuseIntensity);
	//Specular
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(norm, halfwayDir), 0.0), u_Material.shininess);
	vec3 SpecularDirectional = vec3(u_DirectionalLightColor) * spec * vec3(u_Material.specularIntensity);

	return ( (1.0 - shadow) * (DiffuseDirectional + SpecularDirectional) + AmbientDirectional);
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

	vec4 diffuseColor = texture(u_DiffuseMap, v_UV);

	//Takes texture, object base color and metalness into acount.
	vec3 ObjectProperties = vec3(diffuseColor)  * vec3(u_Color) + (vec3(reflection) * u_Material.metalness);

	vec3 viewDir = normalize(u_CameraPos - v_Position);

	//Object looks completely dark when not lit.
	vec3 FinalColor = {0,0,0};

	//Directional Shadow
	float directionalShadow = DirectionalShadowCalculation(v_FragPosLightSpace, v_Position, u_DirectionalLightPosition, v_Normal);
	
	//Add directional light calculation to target pixel
	if (u_CastDirectionalLight > 0.5f)
		FinalColor += CalcDirectionalLight(v_Normal, viewDir, v_Position, directionalShadow) * ObjectProperties;

	//Add point light calculation to target pixel. (Each point light contributes to the final color)
	for (int i = 0; i < u_PointLightCount; i++)
	{
		//Point Light shadow
		float pointShadow = PointShadowCalculation(v_Position, i);
		if (u_PointLights[i].castShadow > 0.5f)
			FinalColor += CalcPointLight(u_PointLights[i], v_Normal, v_Position, viewDir, pointShadow) * ObjectProperties;
		else
			FinalColor += CalcPointLight(u_PointLights[i], v_Normal, v_Position, viewDir, 1) * ObjectProperties;
	}

	//Gamma correction constant.
	float gamma = 2.2;

	//Use this line if you want gamma correction
	//FragColor = vec4(pow(FinalColor, vec3(1.0 / gamma)),1.0);

	FragColor = vec4(FinalColor, 1.0);
	//Integer buffer for mouse picking.
	IntegerColorBuffer = u_EntityID;
}