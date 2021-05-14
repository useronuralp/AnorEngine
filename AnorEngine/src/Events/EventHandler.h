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
			inline static void SetTargetApplication(Application* target) { s_TargetApplication = target; }
			static bool IsKeyDown(int keyCode);
			static bool IsMousePressed(int button);
			static float GetMouseXOffset(GLFWwindow* windowPtr);
			static float GetMouseYOffset(GLFWwindow* windowPtr);
		};
	}
}