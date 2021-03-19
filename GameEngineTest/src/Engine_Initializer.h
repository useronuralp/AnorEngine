#pragma once
#include "pch.h"
namespace GameEngineTest
{
	class ENGINE_API EngineInitializer
	{
	public:
		static bool init()
		{
			//Usually, static classes and must be initialized stuff go here.
			GameEngineTest::Logger::init();
			WARN("ENGINE::{0}", "Following components have been initialized:");
			WARN("ENGINE::{0}", "Logger initialized!");
			WARN("Press enter to continue...");
			getchar();
			return true;
		}
	private:
	};
}