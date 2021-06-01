#include "pch.h"
#include "Timer.h"
namespace AnorEngine
{
	Timer::Timer(const char* name, std::function<void(ProfileResult)> func)
		:m_Name(name), m_Stopped(false), m_CallbackFunc(func)
	{
		m_StartTime = std::chrono::high_resolution_clock::now();
	}
	Timer::~Timer()
	{
		if (!m_Stopped)
			Stop();
	}
	void Timer::Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		m_Stopped = true;
		float duration = (end - start) * 0.001f;
		m_CallbackFunc({ m_Name, duration });
	}
}