#pragma once
namespace GameEngineTest
{
	class ENGINE_API Application
	{
	public:
		Application();
		void Run();
		virtual void Draw();
	protected:
		virtual void logInfo();
	};
}

