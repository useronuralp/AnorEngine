#pragma once
#include "entt.h"
#include "Renderer/Renderer2D.h"
#include "Graphics/Camera.h"
namespace AnorEngine
{
	struct Position2D
	{
		float x;
		float y;
	};
	class Entity;
	class ANOR_API Scene
	{
	public:
		Scene();
		~Scene();
		void OnRender(float deltaTime);
		void OnResizeViewport(uint32_t width, uint32_t height);
		void OnMouseScroll(float xoffset, float yoffset);
		Entity CreateEntity(const std::string& name = std::string());
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