#pragma once
#include <memory>
#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_BUILD_DLL
		#define ANOR_API //__declspec(dllexport)
	#else 
		#define ANOR_API //__declspec(dllimport)
	#endif
#else 
	#error Engine currently supports only windows!!!
#endif

#define WARN(...) AnorEngine::Logger::getConsole()->warn(__VA_ARGS__)
#define CRITICAL_ASSERT(...) AnorEngine::Logger::getConsole()->critical(__VA_ARGS__); __debugbreak()
#define INFO(...) AnorEngine::Logger::getConsole()->info(__VA_ARGS__)

#define PROFILE_SCOPE(name) Timer timer__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })

namespace AnorEngine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T>
	using Unique = std::unique_ptr<T>;

}


