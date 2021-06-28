#include "pch.h"
#include "Application.h"
#include "EngineInitializer.h"
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
		EngineInitializer::init();
		m_OpenGLWindow = std::make_shared<Graphics::OpenGLWindow>("Anor Engine", 1280, 720);  //window creation
		if (!m_OpenGLWindow)
		{
			CRITICAL_ASSERT("APP::{0}", "Application failed to initialize the OpenGLWindow");
		}
		else
			WARN("APP::{0}", "OpenGLWindow was successfuly created");


		Graphics::Renderer::Init(); //Enables Blending for now.
		Graphics::Renderer2D::Init(); //Initializes 2DRenderer for now.

		LogInfoDebug();
	}
	Application::~Application()
	{	
		WARN("Application Destructor!!!");
	}
A	void Application::Close()
	{
		m_OpenGLWindow->Close();
	}
	void Application::LogInfoDebug()
	{
		WARN("APP::{0}", "Application Default Constructor!!");
		WARN("APP::{0}", "Press enter...");
	}
}