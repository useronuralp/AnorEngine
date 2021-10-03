#pragma once
//Forward declerations
class GLFWwindow;

namespace AnorEngine
{
	//Forward declerations
	class Application;
	
	namespace Input
	{
		//Forward declerations
		class Event;

		class ANOR_API EventHandler
		{
		private:
			static Application* s_TargetApplication;
		public:
			static void SubmitEvent(Ref<Event> event);
			//Important to set this to the active Application else, you won't get your input processed.
			inline static void SetTargetApplication(Application* target) { s_TargetApplication = target; }
			static bool IsKeyDown(int keyCode);
			static bool IsMouseButtonDown(int button);
			static double GetMouseXOffset(GLFWwindow* windowPtr);
			static double GetMouseYOffset(GLFWwindow* windowPtr);
		};
	}
}