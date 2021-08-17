#pragma once
#include <glm.hpp>
#include "Graphics/texture.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Graphics/Material.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <gtx/quaternion.hpp>

namespace AnorEngine
{
	struct ANOR_API TransformComponent
	{
		glm::vec3 Translation { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale       { 1.0f, 1.0f, 1.0f };
		glm::vec3 Rotation    { 0.0f, 0.0f, 0.0f };

		int EntityID = -1;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform()
		{
			glm::mat4 transform(1.0f);
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			transform = glm::translate(transform, Translation) * rotation * glm::scale(transform, { Scale.x, Scale.y, Scale.z });
			return transform;
		}
		const glm::mat4 GetTransform() const
		{
			glm::mat4 transform(1.0f);
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			transform = glm::translate(transform, Translation) * rotation * glm::scale(transform, { Scale.x, Scale.y, Scale.z });
			return transform;
		}
	};
	struct ANOR_API SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Graphics::Texture> Texture = std::make_shared<Graphics::Texture>(std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\WhiteTexture.PNG"); //default texture
		glm::vec2 TextureSize = {1.0f, 1.0f};
		glm::vec2 SubTextureOffset = { 1.0f, 1.0f };
		glm::vec2 SubTextureDimensions = { Texture->GetHeight(), Texture->GetWidth() }; //default

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec2& textureSize, const glm::vec4& color)
			:TextureSize(textureSize), Color(color){}
		SpriteRendererComponent(const glm::vec4& color)
			:Color(color) {}
		SpriteRendererComponent(const glm::vec4& color, Ref<Graphics::Texture> texture)
			:Color(color), Texture(texture) {}
		SpriteRendererComponent(const glm::vec4& color, Ref<Graphics::Texture> texture, const glm::vec2& textureSize, const glm::vec2& subTextureOffset, const glm::vec2& subTextureDimensions)
			:Color(color), Texture(texture), TextureSize(textureSize),SubTextureOffset(subTextureOffset), SubTextureDimensions(subTextureDimensions) {}
	};
	struct ANOR_API MeshRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Graphics::Material Material;
		Ref<Graphics::Texture> Texture = std::make_shared<Graphics::Texture>(std::string(__SOLUTION_DIR) + "AnorEngine\\Assets\\Textures\\WhiteTexture.PNG"); //default texture

		MeshRendererComponent() = default;
		MeshRendererComponent(Graphics::Material material)
			:Material(material){}
		MeshRendererComponent(Graphics::Material material, const Ref<Graphics::Texture>& texture)
			:Material(material), Texture(texture) {}
		MeshRendererComponent(glm::vec4 color, Graphics::Material material)
			:Color(color), Material(material){}

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
		NativeScriptComponent(bool enableOnStart = false)
		:isEnabled(enableOnStart){};

		bool isEnabled;
		ScriptableEntity* Instance = nullptr;
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