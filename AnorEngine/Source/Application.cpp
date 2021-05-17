#include "pch.h"
#include "Application.h"
#include "Engine_Initializer.h"
namespace AnorEngine
{	
	Application* Application::s_Instance = nullptr;
	void Application::Run()
	{
		WARN("APP::{0}", "Running!");
	}
	Application::Application()
	{	
		s_Instance = this; //static setup of instance of this singleton class.
		AnorEngine::EngineInitializer::init();
		m_OpenGLWindow = std::make_shared<Graphics::OpenGLWindow>("Anor Engine", 1920, 1080);  //window creation
		if (!m_OpenGLWindow)
		{
			CRITICAL_ASSERT("APP::{0}", "Application failed to initialize the OpenGLWindow");
		}
		else
			WARN("APP::{0}", "OpenGLWindow was successfuly created");


		Graphics::Renderer::Init(); //Enables Blending for now.
		

		logInfoDebug();
	}
	Application::~Application()
	{	
		WARN("Application Destructor!!!");
	}
	void Application::logInfoDebug()
	{
		WARN("APP::{0}", "Application Default Constructor!!");
		WARN("APP::{0}", "Press enter...");
	}
}