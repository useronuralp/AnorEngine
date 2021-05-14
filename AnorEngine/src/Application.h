#pragma once
#include "Graphics/OpenGLWindow.h"
#include "ImGui/ImGuiLayer.h"
namespace AnorEngine
{	
	namespace Input
	{
		class Event; // forward declaring.
	}
	class ANOR_API Application
	{
	protected:
		Ref<Graphics::OpenGLWindow> m_OpenGLWindow;
		Ref<Graphics::ImGuiLayer> ImGuiEditorLayer; //Every application should only have one ImGui UI initializer really.
	private:
		static Application* s_Instance; //static singleton instance of this class.
	public:
		Application();
		virtual ~Application();
	public:
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Ref<Input::Event> event) {};
		virtual void Run();
		inline static Application& Get() { return *s_Instance; }
		inline Ref<Graphics::OpenGLWindow> GetOpenGLWindow() { return m_OpenGLWindow; }
	protected:
		virtual void logInfoDebug();
	};
}

