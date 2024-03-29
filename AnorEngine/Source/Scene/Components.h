#pragma once
#include <glm.hpp>
#include "Graphics/texture.h"
#include "SceneCamera.h"
#include "ScriptableEntity.h"
#include "Graphics/Material.h"
#include "Graphics/model.h"
#include "Renderer/renderer2D.h"
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

	private:
		mutable glm::mat4 Transform = glm::mat4(1.0f);
	public:
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;

		glm::mat4 GetTransform()
		{
			glm::mat4 transform(1.0f);
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			transform = glm::translate(transform, Translation) * rotation * glm::scale(transform, { Scale.x, Scale.y, Scale.z });
			return transform;
		}
		const glm::mat4& GetTransform() const
		{
			Transform = glm::mat4(1.0f);
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			Transform = glm::translate(Transform, Translation) * rotation * glm::scale(Transform, { Scale.x, Scale.y, Scale.z });
			return Transform;
		}
	};
	struct ANOR_API SpriteRendererComponent
	{
		glm::vec4 Color { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Graphics::Texture> Texture = Graphics::Renderer2D::CreateTexture("Textures\\WhiteTexture.png"); //default texture
		glm::vec2 TextureSize = {1.0f, 1.0f};
		glm::vec2 SubTextureOffset = { 0.0f, 0.0f };
		glm::vec2 PixelSizeOfEachCell = { Texture->GetHeight(), Texture->GetWidth() }; //default

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec2& textureSize, const glm::vec4& color)
			:TextureSize(textureSize), Color(color){}
		SpriteRendererComponent(const glm::vec4& color)
			:Color(color) {}
		SpriteRendererComponent(const glm::vec4& color, Ref<Graphics::Texture> texture)
			:Color(color), Texture(texture) {}
		SpriteRendererComponent(const glm::vec4& color, Ref<Graphics::Texture> texture, const glm::vec2& textureSize, const glm::vec2& subTextureOffset, const glm::vec2& subTextureDimensions)
			:Color(color), Texture(texture), TextureSize(textureSize),SubTextureOffset(subTextureOffset), PixelSizeOfEachCell(subTextureDimensions) {}
	};
	struct ANOR_API MeshRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Graphics::Material> Material;
		bool CastDirectionalLightBool = true;
		float CastDirectionalLight = 1.0f; //Treat this as a bool, this is the one being sent to the shader

		MeshRendererComponent() = default;
		MeshRendererComponent(Ref<Graphics::Material> material)
			:Material(material) {}
		MeshRendererComponent(const glm::vec4& color, Ref<Graphics::Material> material)
			:Color(color), Material(material){}
	};

	//----------------
	struct ANOR_API ModelRendererComponent //Temporary
	{
		Ref<Graphics::Model> Model; //Self contains all the necessary textures.
		ModelRendererComponent(Ref<Graphics::Model> model)
			:Model(model){}
	};
	//----------------

	struct ANOR_API PointLightComponent
	{
		float Linear = 0.09f;
		float Constant = 1.0f;
		float Quadratic = 0.32f;
		float Intensity = 1.0f;
		bool CastPointLightShadowBool = true;
		float CastPointLightShadow = 1.0f;

		PointLightComponent() = default;
		PointLightComponent(float Linear, float Constant, float Quadratic, float Intensity)
			:Linear(Linear), Constant(Constant), Quadratic(Quadratic), Intensity(Intensity){}
	};

	struct ANOR_API TagComponent
	{
		std::string Tag;
		std::string Name = "Unnamed Entity";
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) {}

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