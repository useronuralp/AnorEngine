#pragma once
#include "Graphics/window.h"
#include "Graphics/Layers/Layer.h"
namespace GameEngineTest
{
	class ENGINE_API Application
	{
	protected:
		Graphics::Window *m_Window = nullptr;
	private:
		static Application* s_Instance; //static singleton instance of this class.
	public:
		Application();
		virtual ~Application();
	public:
		virtual void Run();
		inline static Application& Get() { return *s_Instance; }
		//inline int getWindowHeight() { return m_Window->getHeight(); }
		//inline int getWindowWidth() { return m_Window->getWidth(); }
		inline GameEngineTest::Graphics::Window& getWindow() { return *m_Window; }
	protected:
		virtual void logInfoDebug();
	};
}

