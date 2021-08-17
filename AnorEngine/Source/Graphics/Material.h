#pragma once
#include "Shader.h"
namespace AnorEngine
{
	namespace Graphics
	{
		struct ANOR_API MaterialProperties
		{
			float		Ambient =  1.0f;
			float		Diffuse =  1.0f;
			float		Specular = 1.0f;
			float		Shininess = 100.0f;
			float		Metalness = 0.0f;
			float		Intensity = 1.0f;
		};
		struct ANOR_API Material
		{
			Material() = default;
			Material(const Material& material) = default;
			Material(const Ref<Shader>& shader)
				:Shader(shader){}
			Material(const Ref<Shader>& shader, MaterialProperties properties)
			:Shader(shader), Properties(properties){}
			//Need deault shader.
			Ref<Shader> Shader;
			MaterialProperties Properties;
		};
	}
}