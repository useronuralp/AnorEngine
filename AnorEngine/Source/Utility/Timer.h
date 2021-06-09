#pragma once
namespace AnorEngine
{
	struct ANOR_API ProfileResult
	{
		const char* Name;
		float Time;
	};

	class ANOR_API Timer
	{
	public:
		Timer(const char* name, std::function<void(ProfileResult)> func);
		~Timer();
	private:
		void Stop();
	private:
		bool m_Stopped;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
		const char* m_Name;
		std::function<void(ProfileResult)> m_CallbackFunc;
	};
}