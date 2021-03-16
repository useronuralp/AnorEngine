//The whole point of defining this entry point here is that enclosing it inside the engine rather than the client. 
//Client should not really know much about the internal code of the engine. 
#pragma once
#include "pch.h"
#include "Application.h"

extern GameEngineTest::Application* CreateApplication();

int main()
{
	auto app = CreateApplication();
	app->Draw();
	delete app;

	return 0;
}