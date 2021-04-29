#pragma once 
#include <map>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <stack>
#include "Layers/Layer.h"
#include <EngineKeyCodes.h>
#include <MouseButtonCodes.h>

namespace GameEngineTest {
	namespace Graphics {

		struct ENGINE_API MousePosition
		{
			double x, y;
		};

		class ENGINE_API Window {
		private:
			//this pointer currently causes high coupling between this "singleton" window class and ImGuiLayer. You should get rid of this dependency and create a robust event handler/dispatcher class in the future.
			Layer *ImGui; 
		private:
			std::map<char, bool> m_Keys = {{'W', false }, {'S', false }, {'A', false}, {'D', false}};
			const char *m_Title;
			int m_Width, m_Height;
			GLFWwindow* m_Window;
			bool m_Closed;
			MousePosition mousePosition;
			bool isMouseCaptured = false;
			float deltaTime = 0.0f;	// Time between current frame and last frame
			float lastFrame = 0.0f; // Time of last frame
			bool isInitialMouseCaptured = false;
			float lastY = m_Height / 2.0f;
			float lastX = m_Width / 2.0f;
			float YAW = -90.0f;
			float PITCH = 0.0f;
			float sensitivity = 0.05f;
		public:
			glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			float cameraSpeed = 0.005f; // adjust accordingly
		public:
			Window(const char* name, int width, int height);
			~Window();
			void clear() const;
			void update();
			bool isClosed() const;
			void drawRightAngledTriangle();
			inline void getMousePosition(double& x, double& y) { x = mousePosition.x; y = mousePosition.y; }
			inline float getHeight() { return m_Height; }
			inline float getWidth() { return m_Width; }
			inline Layer* getImGuiWindowPointer() { return ImGui; }
			inline void setImGuiPointer(Layer* ptr) { ImGui = ptr; }
			inline GLFWwindow* getGLFWwindow() { return m_Window; }
		private:
			bool init();
			friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
			friend static void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		};
	}
}
