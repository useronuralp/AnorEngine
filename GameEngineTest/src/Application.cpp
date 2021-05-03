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
		m_Window = new Graphics::OpenGLWindow("OnuralpEngine", 1920, 1080);  //window creation
		ImGui = new Graphics::ImGuiLayer(); //Create the imgui layer at the construction.
		if (!ImGui)
			CRITICAL("APP::{0}", "Application failed to initialize ImGui Layer!!!!");
		else
			WARN("APP::{0}", "ImGui Layer was successfuly created");
		if (!m_Window)
			CRITICAL("APP::{0}", "Application failed to initialize the window");
		else
			WARN("APP::{0}", "OpenGLWindow was successfuly created");

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