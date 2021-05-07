//The whole point of defining this entry point here is that enclosing it inside the engine rather than the client. 
//Client should not really know much about the internal code of the engine. 
#pragma once
#include "pch.h"
#include "Application.h"
#include "Graphics/Layers/LayerStack.h"
#include <filesystem>
namespace Game
{
	extern AnorEngine::Application* CreateApplication();
}

int main()
{	
	auto app = Game::CreateApplication();
	app->Run();
	delete app;
	system("pause");
	return 0;
}