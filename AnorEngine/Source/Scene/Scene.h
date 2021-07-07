#pragma once
#include <entt.h>
#include <Renderer/Renderer2D.h>
#include <Scene/Components.h>
namespace AnorEngine
{
	class Entity;
	class ANOR_API Scene
	{
	public:
		Scene();
		~Scene();
		void OnRender(float deltaTime);
		Entity CreateEntity(const std::string& name = std::string());
	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}