#include "pch.h"
#include "Application.h"
#include "Engine_Initializer.h"

namespace GameEngineTest
{	
	Application* Application::s_Instance = nullptr;
	void Application::Run()
	{
		WARN("APP::{0}", "Running!");
	}
	Application::Application()
	{	
		s_Instance = this; //static setup of instance of this singleton class.
		GameEngineTest::EngineInitializer::init();
		m_Window = new Graphics::Window("Onuralp_Engine", 1920, 1080); //window creation

		if (!m_Window)
		{
			CRITICAL("APP::{0}", "Application failed to initialize the window");
		}
		else
		{
			WARN("APP::{0}", "Window created");
		}
		logInfoDebug();
	}
	Application::~Application()
	{
		delete m_Window;
	}
	void Application::logInfoDebug()
	{
		WARN("APP::{0}", "Application Constructor!!");
		WARN("APP::{0}", "Press a key...");
		getchar();
	}
}