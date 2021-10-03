#pragma once
#include "Core/Engine.h"
namespace AnorEngine
{
	using namespace Graphics;
	class ExampleLayer : public Layer
	{
		bool			    m_IsRuntime = false;
		Ref<EditorCamera>   m_EditorCamera;
		glm::vec4			m_Color = { 1,1,1,1 };
		Ref<Scene>			m_Scene = std::make_shared<Scene>();
		//Entity1----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_Entity1 = std::make_shared<Entity>(m_Scene->CreateEntity("Diamonds"));
		Ref<Texture>		m_Entity1TextureAtlas = std::make_shared<Texture>("Textures\\PlatformerTextures\\Tilesheet\\platformPack_tilesheet@2.png");
		//Entity2----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_Entity2 = std::make_shared<Entity>(m_Scene->CreateEntity("Block"));
		Ref<Texture>		m_Entity2_Texture = std::make_shared<Texture>("Textures\\381f5a63791945.5abc4ccf1297d.png");
		//Entity3----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_Entity3 = std::make_shared<Entity>(m_Scene->CreateEntity("Player"));
		Ref<Texture>		m_Entity3TextureAtlas = std::make_shared<Texture>("Textures\\PlatformerTextures\\Tilesheet\\platformerPack_character@2.png");
		//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		Ref<Entity>			m_CameraEntity = std::make_shared<Entity>(m_Scene->CreateEntity("Camera"));
	public:
		Ref<Scene>& GetScene() { return m_Scene; }

		ExampleLayer(Ref<EditorCamera>& camera)
			:m_EditorCamera(camera) {}
		virtual void OnAttach() override;
		virtual void OnUpdate(bool IsRuntime, float deltaTime) override;
		virtual void OnEvent(Ref<Input::Event> e) override;
		virtual void OnResizeViewport(uint32_t width, uint32_t height) override;
		virtual void OnMouseScroll(float xoffset, float yoffset) override;
	};
}