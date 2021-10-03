#include "pch.h"
#include "Application.h"
#include "Graphics/OpenGLWindow.h"
#include "Renderer/Renderer2D.h"
#include "Graphics/Layers/ImGuiBase.h"
#include "EngineInitializer.h"

namespace AnorEngine
{	
	Application* Application::s_Instance = nullptr;
	void Application::Run()
	{
		WARN("APP::{0}", "Running!");
	}
	Application::Application(const char* appName)
	{	
		s_Instance = this; //static setup of instance of this singleton class.
		m_OpenGLWindow = std::make_shared<Graphics::OpenGLWindow>(appName, 1920, 1080); //window creation
		EngineInitializer::init();
		ImGuiBase::Init(); // Need to call the initialization code for imgui here. 
		if (!m_OpenGLWindow)
		{
			CRITICAL_ASSERT("APP::{0}", "Application failed to initialize the OpenGLWindow");
		}
		else
			WARN("APP::{0}", "OpenGLWindow was successfuly created");
		Graphics::Renderer2D::Init(); //Initializes 2DRenderer for now.
		LogInfoDebug();
	}
	Application::~Application()
	{	
		WARN("Application Destructor!!!");
	}
	void Application::Close()
	{
		m_OpenGLWindow->Close();
	}
	void Application::LogInfoDebug()
	{
		WARN("APP::{0}", "Application Default Constructor!!");
		WARN("APP::{0}", "Press enter...");
	}
}