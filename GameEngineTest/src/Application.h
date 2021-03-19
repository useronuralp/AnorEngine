#pragma once
#include "Graphics/window.h"
namespace GameEngineTest
{
	class ENGINE_API Application
	{
	protected:
		Graphics::Window *m_Window = nullptr;
	private:
		static Application* s_Instance;
	public:
		Application();
		~Application();
		virtual void Run();
		inline static Application& Get() { return *s_Instance; }
		//inline int getWindowHeight() { return m_Window->getHeight(); }
		//inline int getWindowWidth() { return m_Window->getWidth(); }
		inline GameEngineTest::Graphics::Window& getWindow() { return *m_Window; }
	protected:
		virtual void logInfoDebug();
	};
}

