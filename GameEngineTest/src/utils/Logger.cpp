#include "pch.h"
#include "Logger.h"

namespace GameEngineTest
{
	std::shared_ptr<spdlog::logger> Logger::console;
	void Logger::init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		console = spdlog::stdout_color_mt("console");
	}
}