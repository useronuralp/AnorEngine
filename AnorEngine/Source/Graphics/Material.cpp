#include "pch.h"
#include "Material.h"
#include "Renderer/renderer2D.h"
#include "Shader.h"
#include "texture.h"
namespace AnorEngine
{
	namespace Graphics
	{
		Material::Material()
		{
			Init();
		}
		Material::Material(Ref<Shader> shader)
		{
			Init();
			m_Shader = shader;
		}
		Material::Material(const Ref<Shader>& shader, const Ref<Graphics::Texture>& texture)
		{
			m_Shader = shader;
			m_Texture = texture;
		}
		Material::Material(const Ref<Shader>& shader, MaterialProperties properties, const Ref<Graphics::Texture>& texture)
		{
			m_Shader = shader;
			m_Properties = properties;
			m_Texture = texture;
		}
		void Material::Init()
		{
			m_Texture = Graphics::Renderer2D::CreateTexture("Textures\\WhiteTexture.png"); //default texture
		}
	}
}