#include "pch.h"
#include "Scene.h"
#include "Entity.h"
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
		Graphics::Camera* mainCamera = nullptr;
		glm::mat4* mainCameraTransform = nullptr; 
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto& entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				mainCameraTransform = &transform.Transform;
				break;
			}
		}
		if (mainCamera)
		{
			Graphics::Renderer2D::BeginScene(mainCamera, *mainCameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto& entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Graphics::Renderer2D::Submit(transform, sprite.Size, sprite.Texture, sprite.SubTextureOffset, sprite.SubTextureDimensions, 0.0f, sprite.Color);
			}
			Graphics::Renderer2D::EndScene();
		}
	}
	void Scene::OnResizeViewport(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				
				cameraComponent.Camera.SetViewportSize(width, height);
			}
		}

	}
	void Scene::OnMouseScroll(float xoffset, float yoffset)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetOrthographic(cameraComponent.Camera.GetOrhographicSize() - yoffset, -1.0f, 1.0f);
			}
		}
	}
	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Unnamed Entity" : name;
		return entity;
	}
}

