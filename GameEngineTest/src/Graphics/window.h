#pragma once 
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace GameEngineTest {
	namespace Graphics {

		struct MousePosition
		{
			double x, y;
		};

		class Window {
		private:
			
			const char *m_Title;
			int m_Width, m_Height;
			GLFWwindow* m_Window;
			bool m_Closed;
			MousePosition mousePosition;
		public:
			Window(const char* name, int width, int height);
			~Window();
			void clear() const;
			void update();
			bool closed() const;
			void drawRightAngledTriangle();
			inline void getMousePosition(double& x, double& y) { x = mousePosition.x; y = mousePosition.y; }

		private:
			bool init();
			friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

		};
	}
}
