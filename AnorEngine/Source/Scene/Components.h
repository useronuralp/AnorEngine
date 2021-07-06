#pragma once
#include <glm.hpp>
#include "Graphics/texture.h"
namespace AnorEngine
{
	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			:Transform(transform) {}
		void Translate(float x, float y, float z)
		{
			Transform = glm::translate(Transform, glm::vec3{ x,y,z });
		}
		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};
	struct SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Graphics::Texture> Texture;
		glm::vec2 Size = {1.0f, 1.0f};
		glm::vec2 SubTextureOffset = { 1.0f, 1.0f };
		glm::vec2 SubTextureDimensions = { Texture->GetHeight(), Texture->GetWidth() };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			:Color(color) {}
		SpriteRendererComponent(const glm::vec4& color, Ref<Graphics::Texture> texture)
			:Color(color), Texture(texture) {}
		SpriteRendererComponent(const glm::vec4& color, Ref<Graphics::Texture> texture, glm::vec2 size, glm::vec2 subTextureOffset, glm::vec2 subTextureDimensions)
			:Color(color), Texture(texture), Size(size),SubTextureOffset(subTextureOffset), SubTextureDimensions(subTextureDimensions) {}
	};
}