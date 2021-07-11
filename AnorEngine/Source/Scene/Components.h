#pragma once
#include <glm.hpp>
#include "Graphics/texture.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
namespace AnorEngine
{
	struct ANOR_API TransformComponent
	{
		glm::mat4 Transform { 1.0f };

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
	struct ANOR_API SpriteRendererComponent
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
	struct ANOR_API TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& name)
			: Tag(name) {}

	};
	struct ANOR_API CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct ANOR_API NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		//These std::function's will bind to the functions of whatever T class you pass here. This way you'll be able to call those functions in a script.
		ScriptableEntity* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}