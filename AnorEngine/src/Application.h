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
		Graphics::OpenGLWindow *m_OpenGLWindow = nullptr;
		Graphics::ImGuiLayer* ImGui = nullptr; //Every application should only have one ImGui UI initializer really.
	private:
		static Application* s_Instance; //static singleton instance of this class.
	public:
		Application();
		virtual ~Application();
	public:
		virtual void OnEvent(Input::Event* event){}
		virtual void Run();
		inline static Application& Get() { return *s_Instance; }
		inline Graphics::OpenGLWindow& GetOpenGLWindow() { return *m_OpenGLWindow; }
	protected:
		virtual void logInfoDebug();
	};
}

