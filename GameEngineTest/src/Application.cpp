#include "pch.h"
#include "Application.h"
#include "Engine_Initializer.h"

namespace GameEngineTest
{	
	void Application::Run()
	{
		WARN("APP::{0}", "Running!");
	}
	Application::Application()
	{	
		GameEngineTest::EngineInitializer::init();
	}
	void Application::logInfo()
	{
		WARN("APP::{0}", "Application Constructor!!");
		WARN("APP::{0}", "Press a key...");
		getchar();
	}
	void Application::Draw() {};
}