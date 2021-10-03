#pragma once
#include "entt.h"
namespace AnorEngine
{
	//Forward declerations
	class Entity;
	class Camera;
	namespace Graphics
	{
		class EditorCamera;
	}

	struct Position2D
	{
		float x;
		float y;
	};
	class ANOR_API Scene
	{
	public:
		Scene();
		~Scene();
		entt::registry& GetRegistry() { return m_Registry; };
		void OnUpdateRuntime(float deltaTime);
		void OnUpdateEditor(float deltaTime, Ref<Graphics::EditorCamera> camera);
		void OnResizeViewport(uint32_t width, uint32_t height);
		void OnMouseScroll(float xoffset, float yoffset);
		Entity CreateEntity(const std::string& name = std::string(), const std::string& tag = std::string());
		Position2D GetPlayerLocation();
		void DestroyEntity(Entity entity);
	private:
		entt::registry m_Registry;
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchyPanel;
	};
}