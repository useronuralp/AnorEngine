#pragma once
namespace AnorEngine
{
	class ANOR_API FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}