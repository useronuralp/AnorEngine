#pragma once
#include "Graphics/OpenGLWindow.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
namespace AnorEngine
{	
	namespace Input
	{
		//TODO: Remove these
		class Event; // forward declaring.
		class WindowResizeEvent;
	}
	class ANOR_API Application
	{
	protected:
		Ref<Graphics::OpenGLWindow> m_OpenGLWindow;
	private:
		static Application* s_Instance; //static singleton instance of this class.
	public:
		Application();
		virtual ~Application();
	public:
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Ref<Input::Event> event) {};
		virtual void OnWindowResizeEvent(Ref<Input::WindowResizeEvent> event) {};
		virtual void Run();
		inline static Application& Get() { return *s_Instance; }
		inline Ref<Graphics::OpenGLWindow> GetOpenGLWindow() { return m_OpenGLWindow; }
	protected:
		virtual void LogInfoDebug();
	};
}

