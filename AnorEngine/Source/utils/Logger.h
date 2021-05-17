#pragma once
#include "Logger.h" //you can write your own wrapper for this library in the future.
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#define WARN(...) AnorEngine::Logger::getConsole()->warn(__VA_ARGS__)
#define CRITICAL_ASSERT(...) AnorEngine::Logger::getConsole()->critical(__VA_ARGS__); __debugbreak()
#define INFO(...) AnorEngine::Logger::getConsole()->info(__VA_ARGS__)

namespace AnorEngine {
	
	class ANOR_API Logger
	{
	private:
		static std::shared_ptr<spdlog::logger> console;
		Logger() = delete;
	public:
		static void init();
	public:
		static std::shared_ptr<spdlog::logger>& getConsole() { return console; }
	};

}

