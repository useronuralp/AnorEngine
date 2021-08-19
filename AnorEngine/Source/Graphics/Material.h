#pragma once
#include "Shader.h"
#include "texture.h"
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
		};
		struct ANOR_API Material
		{
			Material() = default;
			Material(const Material& material) = default;
			Material(Ref<Shader> shader)
				:Shader(shader) {}
			Material(const Ref<Shader>& shader, const Ref<Graphics::Texture>& texture)
				:Shader(shader), Texture(texture){}
			Material(const Ref<Shader>& shader, MaterialProperties properties, const Ref<Graphics::Texture>& texture)
			:Shader(shader), Properties(properties), Texture(texture){}
			//Need deault shader.
			Ref<Shader> Shader;
			MaterialProperties Properties;
			Ref<Graphics::Texture> Texture = std::make_shared<Graphics::Texture>(std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\WhiteTexture.PNG"); //default texture
		};
	}
}