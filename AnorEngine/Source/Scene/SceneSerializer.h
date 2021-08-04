#pragma once
#include <yaml-cpp/yaml.h>
namespace AnorEngine
{
	class Scene;
	struct ANOR_API SceneSerializer
	{
		SceneSerializer(const Ref<Scene>& scene);
		void Serialize(const std::string& filepath);
		void SerializeRuntime(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
		void DeserializeTuntime(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}