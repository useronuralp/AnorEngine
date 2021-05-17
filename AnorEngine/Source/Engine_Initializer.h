#pragma once
#include "pch.h"
namespace AnorEngine
{
	class ANOR_API EngineInitializer
	{
	public:
		static bool init()
		{
			//Usually, static classes and must be initialized stuff go here.
			AnorEngine::Logger::init();
			WARN("ENGINE::{0}", "Following components have been initialized:");
			WARN("ENGINE::{0}", "Logger initialized!");
			WARN("..............................");
			return true;
		}
	private:
	};
}