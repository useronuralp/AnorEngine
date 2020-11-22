#pragma once 
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <vector>
#include <map>
namespace GameEngineTest {
	namespace Graphics {

		struct MousePosition
		{
			double x, y;
		};

		class Window {
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
			float currentFrame;
			bool firstMouse = true;
			float lastY = m_Height / 2.0f;
			float lastX = m_Width / 2.0f;
			float YAW = -90.0f;
			float PITCH = 0.0f;
			float sensitivity = 0.05f;
		public:
			glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
			glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
			const float cameraSpeed = 0.001f; // adjust accordingly
		public:
			Window(const char* name, int width, int height);
			~Window();
			void clear() const;
			void update();
			bool closed() const;
			void drawRightAngledTriangle();
			inline void getMousePosition(double& x, double& y) { x = mousePosition.x; y = mousePosition.y; }
			inline int getHeight() { return m_Height; }
			inline int getWidth() { return m_Width; }
		private:
			bool init();
			friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

		};
	}
}
