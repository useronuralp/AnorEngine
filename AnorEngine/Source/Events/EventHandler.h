#pragma once
#include "Event.h"
#include "Core/Application.h"
namespace AnorEngine
{
	namespace Input
	{
		class ANOR_API EventHandler
		{
		private:
			static Application* s_TargetApplication;
		public:
			static void SubmitEvent(Ref<Event> event);
			//Important to set this to the active Application else, you won't get your input processed.
			inline static void SetTargetApplication(Application* target) { s_TargetApplication = target; }
			static bool IsKeyDown(int keyCode);
			static bool IsMousePressed(int button);
			static double GetMouseXOffset(GLFWwindow* windowPtr);
			static double GetMouseYOffset(GLFWwindow* windowPtr);
		};
	}
}