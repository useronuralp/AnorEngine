#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"
namespace AnorEngine
{
	Scene::Scene()
	{
	}
	Scene::~Scene()
	{
	}
	void Scene::OnUpdateEditor(float deltaTime, Ref<Graphics::EditorCamera> camera)
	{
		Graphics::Renderer2D::BeginScene(camera);
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto& entity : group)
		{
			auto [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
			Graphics::Renderer2D::Submit(transformComponent, spriteRendererComponent);
		}
		Graphics::Renderer2D::EndScene();
	}
	void Scene::OnUpdateRuntime(float deltaTime)
	{
		//Exaplanation: In this OnRender function, the user can retrieve whatever component they want and do operations on them.

		//Update scripts.
		//This part requires some deep knowledge of C++ to really wrap your head around it. It just calls the bound functions of the scriptComponent though. 
		//Function binding part is what really makes the whole thing complicated.
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Entity = Entity { entity, this };
					nsc.Instance->OnCreate();
				}
				if(nsc.isEnabled)
					nsc.Instance->OnUpdate(deltaTime);
			});
		}

		//Retrieving the camera component of each entity in the scene. What you do after you retrieve them is up to you / to user.
		Graphics::Camera* mainCamera = nullptr;
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		glm::mat4 mainCameraTransform;
		for (auto& entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
			if (camera.Primary)
			{
				mainCamera = &camera.Camera;
				mainCameraTransform = transform.GetTransform();
				break;
			}
		}

		//Retrieving sprite renderer component here so that we can send the corresponding data to the renderer2D.
		if (mainCamera)
		{
			Graphics::Renderer2D::BeginScene(mainCamera, mainCameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto& entity : group)
			{
				auto [transformComponent, spriteRendererComponent] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Graphics::Renderer2D::Submit(transformComponent, spriteRendererComponent);
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
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
	void Scene::OnMouseScroll(float xoffset, float yoffset)
	{
		auto view = m_Registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetOrthographic(cameraComponent.Camera.GetOrhographicSize() - yoffset, -1.0f, 1.0f);
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
	Position2D Scene::GetPlayerLocation()
	{
		auto view = m_Registry.view<TransformComponent, TagComponent>();
		for (auto& entity : view)
		{
			auto [transform, tag] = view.get<TransformComponent, TagComponent>(entity);
			if (tag.Tag == "Player")
			{			
				return Position2D { transform.Translation.x, transform.Translation.y };
			}
		}
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}
}

