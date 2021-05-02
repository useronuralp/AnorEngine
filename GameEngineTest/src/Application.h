#pragma once
#include "Graphics/OpenGLWindow.h"
namespace GameEngineTest
{	
	class ENGINE_API Application
	{
	protected:
		Graphics::OpenGLWindow *m_Window = nullptr;
	private:
		static Application* s_Instance; //static singleton instance of this class.
	public:
		Application();
		virtual ~Application();
	public:
		virtual void Run();
		inline static Application& Get() { return *s_Instance; }
		inline Graphics::OpenGLWindow& getWindow() { return *m_Window; }
	protected:
		virtual void logInfoDebug();
	};
}

