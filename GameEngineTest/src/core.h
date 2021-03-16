#pragma once
#ifdef ENGINE_PLATFORM_WINDOWS
	#ifdef ENGINE_BUILD_DLL
		#define ENGINE_API __declspec(dllexport)
	#else 
		#define ENGINE_API __declspec(dllimport)
	#endif
#else 
	#error Engine currently supports only windows!!!
#endif




