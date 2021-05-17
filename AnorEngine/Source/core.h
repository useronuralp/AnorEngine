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

namespace AnorEngine
{
	template<typename T>
	using Ref = std::shared_ptr<T>;

	template <typename T>
	using Unique = std::unique_ptr<T>;

}


