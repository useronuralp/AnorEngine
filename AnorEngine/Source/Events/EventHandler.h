#pragma once
#include "Event.h"
#include <Application.h>
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
			static float GetMouseXOffset(GLFWwindow* windowPtr);
			static float GetMouseYOffset(GLFWwindow* windowPtr);
		};
	}
}