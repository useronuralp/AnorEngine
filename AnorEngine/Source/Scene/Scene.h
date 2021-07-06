#pragma once
#include <entt.h>
#include <Renderer/Renderer2D.h>
#include <Scene/Components.h>
namespace AnorEngine
{
	class ANOR_API Scene
	{
	public:
		entt::registry& GetReg() { return m_Registry; }
	public:
		Scene();
		~Scene();
		void OnRender(float deltaTime);
		entt::entity CreateEntity();
	private:
		entt::registry m_Registry;
	};
}