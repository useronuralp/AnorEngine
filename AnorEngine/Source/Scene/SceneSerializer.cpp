#include "pch.h"
#include "SceneSerializer.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node encode(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};

}

namespace AnorEngine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}
	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; // Entity
		out << YAML::Key << "Entity" << YAML::Value << "12837192831273"; // TODO: Entity ID goes here
		
		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; // TagComponent
		
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
		
			out << YAML::EndMap; // TagComponent
		}
		
		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; // TransformComponent
		
			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;
		
			out << YAML::EndMap; // TransformComponent
		}
		
		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; // CameraComponent
		
			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;
		
			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; // Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetNearClipPerspective();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetFarClipPerspective();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrhographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetNearClipOrthographic();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetFarClipOrthographic();
			out << YAML::EndMap; // Camera
		
			out << YAML::Key << "Primary" << YAML::Value << cameraComponent.Primary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;
		
			out << YAML::EndMap; // CameraComponent
		}
		
		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap; // SpriteRendererComponent
		
			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;
			out << YAML::Key << "Texture" << YAML::Value << spriteRendererComponent.Texture->GetPath();
			out << YAML::Key << "TextureSize" << spriteRendererComponent.TextureSize;
			out << YAML::Key << "SubTextureOffset" << YAML::Value << spriteRendererComponent.SubTextureOffset;
			out << YAML::Key << "SubTextureDimensions" << YAML::Value << spriteRendererComponent.SubTextureDimensions;
		
			out << YAML::EndMap; // SpriteRendererComponent
		}
		if (entity.HasComponent<MeshRendererComponent>())
		{
			out << YAML::Key << "MeshRendererComponent";
			out << YAML::BeginMap; // MeshRendererComponent
		
			auto& meshRendererComponent = entity.GetComponent<MeshRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << meshRendererComponent.Color;
			out << YAML::Key << "Shader" << YAML::Value << meshRendererComponent.Material->Shader->GetName();
			out << YAML::Key << "Ambient" << YAML::Value << meshRendererComponent.Material->Properties.Ambient;
			out << YAML::Key << "Diffuse" << YAML::Value << meshRendererComponent.Material->Properties.Diffuse;
			out << YAML::Key << "Specular" << YAML::Value << meshRendererComponent.Material->Properties.Specular;
			out << YAML::Key << "Shininess" << YAML::Value << meshRendererComponent.Material->Properties.Shininess;
			out << YAML::Key << "Metalness" << YAML::Value << meshRendererComponent.Material->Properties.Metalness;
			out << YAML::Key << "Texture" << YAML::Value << meshRendererComponent.Material->Texture->GetPath();

			out << YAML::EndMap; // MeshRendererComponent
		}
		if (entity.HasComponent<PointLightComponent>())
		{
			out << YAML::Key << "PointLightComponent";
			out << YAML::BeginMap; // PointLightComponent

			auto& pointLightComponent = entity.GetComponent<PointLightComponent>();
			out << YAML::Key << "Linear" << YAML::Value << pointLightComponent.Linear;
			out << YAML::Key << "Constant" << YAML::Value << pointLightComponent.Constant;
			out << YAML::Key << "Quadratic" << YAML::Value << pointLightComponent.Quadratic;
			out << YAML::Key << "Intensity" << YAML::Value << pointLightComponent.Intensity;

			out << YAML::EndMap; // PointLightComponent
		}
		out << YAML::EndMap; // Entity
	}
	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		:m_Scene(scene)
	{

	}
	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Untitled";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(filepath);
		fout << out.c_str();
	}
	void SceneSerializer::SerializeRuntime(const std::string& filepath)
	{
	}
	bool SceneSerializer::Deserialize(const std::string& filepath)
	{
		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();


				Entity deserializedEntity = m_Scene->CreateEntity(name);

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					// Entities always have transforms
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto cameraProps = cameraComponent["Camera"];
					cc.Camera.SetProjectionType((ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.Camera.SetPerspectiveFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetNearClipPerspective(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetFarClipPerspective(cameraProps["PerspectiveFar"].as<float>());

					cc.Camera.SetOrthoGraphicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetNearClipOrthographic(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetFarClipOrthographic(cameraProps["OrthographicFar"].as<float>());

					cc.Primary = cameraComponent["Primary"].as<bool>();
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					Ref<Graphics::Texture> Texture = std::make_shared<Graphics::Texture>(spriteRendererComponent["Texture"].as<std::string>());
					glm::vec4 color = spriteRendererComponent["Color"].as<glm::vec4>();
					glm::vec2 textureSize = spriteRendererComponent["TextureSize"].as<glm::vec2>();
					glm::vec2 subTextureOffset = spriteRendererComponent["SubTextureOffset"].as<glm::vec2>();
					glm::vec2 subTextureDimensions = spriteRendererComponent["SubTextureDimensions"].as<glm::vec2>();
					auto& src = deserializedEntity.AddComponent<SpriteRendererComponent>(color, Texture, textureSize, subTextureOffset, subTextureDimensions);
				}
				auto meshRendererComponent = entity["MeshRendererComponent"];
				if (meshRendererComponent)
				{
					glm::vec4 color = meshRendererComponent["Color"].as<glm::vec4>();
					Ref<Graphics::Material> material = std::make_shared<Graphics::Material>();
					material->Shader = Graphics::ShaderLibrary::GetShader(meshRendererComponent["Shader"].as<std::string>());
					material->Properties.Ambient = meshRendererComponent["Ambient"].as<float>();
					material->Properties.Diffuse =meshRendererComponent["Diffuse"].as<float>();
					material->Properties.Specular =meshRendererComponent["Specular"].as<float>();
					material->Properties.Shininess =meshRendererComponent["Shininess"].as<float>();
					material->Properties.Metalness =meshRendererComponent["Metalness"].as<float>();
					material->Texture->CreateTextureWithRelativePath(meshRendererComponent["Texture"].as<std::string>());
					auto& src = deserializedEntity.AddComponent<MeshRendererComponent>(color, material);
				}
				auto pointLightComponent = entity["PointLightComponent"];
				if (pointLightComponent)
				{
					float linear = pointLightComponent["Linear"].as<float>();
					float constant = pointLightComponent["Constant"].as<float>();
					float quadratic = pointLightComponent["Quadratic"].as<float>();
					float intensity = pointLightComponent["Intensity"].as<float>();
					auto& src = deserializedEntity.AddComponent<PointLightComponent>(linear, constant, quadratic, intensity);
				}
			}
		}

		return true;
	}
	void SceneSerializer::DeserializeTuntime(const std::string& filepath)
	{
	}
}