#include "pch.h"
#include "EventHandler.h"
namespace AnorEngine
{
	namespace Input
	{
		Application* EventHandler::s_TargetApplication;
		void EventHandler::SubmitEvent(Ref<Event> event)
		{
			EventHandler::s_TargetApplication->OnEvent(event);
		}
		bool EventHandler::IsKeyDown(int keyCode)
		{
			return glfwGetKey(EventHandler::s_TargetApplication->GetOpenGLWindow()->GetNativeWindow(), keyCode); //GLFW_RELEASE equals to 0 thats why this works.
		}
		double EventHandler::GetMouseXOffset(GLFWwindow* windowPtr)
		{
			double xpos, ypos;
			glfwGetCursorPos(windowPtr, &xpos, &ypos);
			return xpos;
		}
		double EventHandler::GetMouseYOffset(GLFWwindow* windowPtr)
		{
			double xpos, ypos;
			glfwGetCursorPos(windowPtr, &xpos, &ypos);
			return ypos;
		}
		bool EventHandler::IsMousePressed(int button)
		{
			return glfwGetMouseButton(s_TargetApplication->GetOpenGLWindow()->GetNativeWindow(), button);
		}
	}
}