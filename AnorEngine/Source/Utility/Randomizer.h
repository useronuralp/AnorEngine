#pragma once
#include <chrono>
namespace AnorEngine
{
	static struct Random
	{
		Random() { srand(time(NULL)); }
		static float GetFloat() { return static_cast <float> (rand()) / static_cast <float> (RAND_MAX); }
	};
}