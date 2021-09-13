#include "pch.h"
#include "ExampleLayer.h"
namespace AnorEngine
{
	void ExampleLayer::OnAttach() 
	{
		struct CameraController : public ScriptableEntity
		{
			void OnCreate()
			{
			}
			void OnDestroy()
			{
			}
			void OnUpdate(float deltaTime)
			{
				if (Input::EventHandler::IsKeyDown(ANOR_KEY_W))
					GetComponent<TransformComponent>().Translation.y += 10.0f * deltaTime;
				else if (Input::EventHandler::IsKeyDown(ANOR_KEY_S))
					GetComponent<TransformComponent>().Translation.y -= 10.0f * deltaTime;
				if (Input::EventHandler::IsKeyDown(ANOR_KEY_A))
					GetComponent<TransformComponent>().Translation.x -= 10.0f * deltaTime;
				else if (Input::EventHandler::IsKeyDown(ANOR_KEY_D))
					GetComponent<TransformComponent>().Translation.x += 10.0f * deltaTime;
			}
		};
		struct CharacterController : public ScriptableEntity
		{
			void OnCreate()
			{
			}
			void OnDestroy()
			{
			}
			void OnUpdate(float deltaTime)
			{
				if (Input::EventHandler::IsKeyDown(ANOR_KEY_I))
					GetComponent<TransformComponent>().Translation.y += 5.0f * deltaTime;
				else if (Input::EventHandler::IsKeyDown(ANOR_KEY_K))
					GetComponent<TransformComponent>().Translation.y -= 5.0f * deltaTime;
				if (Input::EventHandler::IsKeyDown(ANOR_KEY_J))
					GetComponent<TransformComponent>().Translation.x -= 5.0f * deltaTime;
				else if (Input::EventHandler::IsKeyDown(ANOR_KEY_L))
					GetComponent<TransformComponent>().Translation.x += 5.0f * deltaTime;
			}
		};
		struct EnemyBehaviour : public ScriptableEntity
		{
			void OnCreate()
			{
			}
			void OnDestroy()
			{
			}
			void OnUpdate(float deltaTime)
			{

				auto [playerX, playerY] = GetPlayerLocation();
				auto& position = GetComponent<TransformComponent>().Translation;

				if (abs(playerX - position.x) > 0 && abs(playerY - position.y) > 0)
				{
					position.x += (0.75f * (playerX - position.x)) * deltaTime;
					position.y += (0.75f * (playerY - position.y)) * deltaTime;
				}
			}
		};

		if (m_CameraEntity)
		{
			m_CameraEntity->AddComponent<CameraComponent>();
			//You can initialize scripts by passing a bool to their constructors. If it is true the script is enabled upon engine launch. Default value is false.
			m_CameraEntity->AddComponent<NativeScriptComponent>(true).Bind<CameraController>();
		}
		if (m_Entity1)
		{
			m_Entity1->GetComponent<TransformComponent>().Translation.x = 3.5f;
			m_Entity1->GetComponent<TransformComponent>().Translation.y = 3.5f;
			m_Entity1->AddComponent<SpriteRendererComponent>(m_Color, m_Entity1TextureAtlas, glm::vec2{ 2.0f, 1.0f }, glm::vec2{ 9, 3 }, glm::vec2{ 128.0f, 128.0f });
			m_Entity1->AddComponent<NativeScriptComponent>().Bind<EnemyBehaviour>();
		}
		if (m_Entity2)
		{
			m_Entity2->GetComponent<TransformComponent>().Translation.x = -3.5f;
			m_Entity2->GetComponent<TransformComponent>().Translation.y = -3.5f;
			m_Entity2->AddComponent<SpriteRendererComponent>(m_Color, m_Entity2_Texture);
		}
		if (m_Entity3)
		{
			m_Entity3->GetComponent<TagComponent>().Tag = "Player";
			m_Entity3->AddComponent<SpriteRendererComponent>(m_Color, m_Entity3TextureAtlas, glm::vec2{ 1.0f, 1.0f }, glm::vec2{ 0 ,1 }, glm::vec2{ 192.0f, 175.0f });
			m_Entity3->AddComponent<NativeScriptComponent>(true).Bind<CharacterController>();
		}
	}
	void ExampleLayer::OnUpdate(bool IsRuntime, float deltaTime)
	{
		IsRuntime ? m_Scene->OnUpdateRuntime(deltaTime) : m_Scene->OnUpdateEditor(deltaTime, m_EditorCamera);
	}
	void ExampleLayer::OnEvent(Ref<Input::Event> e)
	{
		if (!e->m_Handled)
		{
			////This resizing event handling is currently not needed since the viewport code does the resizing and sends it to the framebuffer.
			////However, I will leave this code here in case the rendering target changes from a framebuffer to a glfw window, then, I would need this resizing event to properly resize the window
			////and if I delete it now, I will have a hard time trying to find the reason why the window won't resize in the future.
			//if (e->GetEventType() == Input::EventType::WindowResizeEvent)
			//{
			//	auto castEvent = std::static_pointer_cast<Input::WindowResizeEvent>(e);
			//	OnResizeViewport(castEvent->GetWidth(), castEvent->GetHeight());
			//	e->m_Handled = true;
			//}
			//if (e->GetEventType() == Input::EventType::MouseScrollEvent)
			//{
			//	auto castEvent = std::static_pointer_cast<Input::MouseScrollEvent>(e);
			//	OnMouseScroll(castEvent->GetXOffset(), castEvent->GetYOffset());
			//	e->m_Handled = true;
			//}
		}
	}
	void ExampleLayer::OnResizeViewport(uint32_t width, uint32_t height)
	{
		m_Scene->OnResizeViewport(width, height);
	}
	void ExampleLayer::OnMouseScroll(float xoffset, float yoffset)
	{
		m_Scene->OnMouseScroll(xoffset, yoffset);
	}
}