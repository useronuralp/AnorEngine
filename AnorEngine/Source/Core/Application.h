#pragma once
#include "Graphics/OpenGLWindow.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Graphics/Layers/ImGuiBase.h"
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
		Application(const char* appName = "Anor Engine");
		virtual ~Application();
	public:
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Ref<Input::Event> event) {};
		virtual void Close();
		virtual void Run();
	public:
		inline static Application& Get() { return *s_Instance; }
		inline Ref<Graphics::OpenGLWindow> GetOpenGLWindow() { return m_OpenGLWindow; }
	protected:
		virtual void LogInfoDebug();
	};
}

