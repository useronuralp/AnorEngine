#include "pch.h"
#include "Scene.h"
namespace AnorEngine
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}
	void Scene::OnRender(float deltaTime)
	{
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto& entity : group)
		{
			auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Graphics::Renderer2D::Submit(transform, sprite.Size, sprite.Texture, sprite.SubTextureOffset, sprite.SubTextureDimensions, 0.0f, sprite.Color);
		}
	}
	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}
}

