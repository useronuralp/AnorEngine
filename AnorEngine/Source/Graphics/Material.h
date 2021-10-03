#pragma once
namespace AnorEngine
{
	namespace Graphics
	{
		//Forward declerations
		class Shader;
		class Texture;

		struct ANOR_API MaterialProperties
		{
			float Ambient =  0.1f;
			float Diffuse =  1.0f;
			float Specular = 1.0f;
			float Shininess = 100.0f;
			float Metalness = 0.0f;
		};
		struct ANOR_API Material
		{
			Material();
			Material(const Material& material) = default;
			Material(Ref<Shader> shader);
			Material(const Ref<Shader>& shader, const Ref<Graphics::Texture>& texture);
			Material(const Ref<Shader>& shader, MaterialProperties properties, const Ref<Graphics::Texture>& texture);
			void Init();
			//TODO : Need deault shader.
			Ref<Shader> m_Shader;
			MaterialProperties m_Properties;
			Ref<Graphics::Texture> m_Texture;
		};
	}
}