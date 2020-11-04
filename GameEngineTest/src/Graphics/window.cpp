#include "window.h"

namespace GameEngineTest {
	namespace Graphics {

		

		Window::Window(const char* title, int width, int height) 
		{
			m_Title = title;
			m_Height = height;
			m_Width = width;
			
			if (!init()) {
				glfwTerminate();
			}
		}

		Window::~Window() 
		{	
			glfwTerminate();
		}
		

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) //friend definition
		{
			//Window* win =  (Window*) glfwGetWindowUserPointer(window);
			//std::cout << "Pressed key code: " << key << std::endl;
		}

		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) //friend definition
		{
			//Window* win =  (Window*) glfwGetWindowUserPointer(window);
			if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
				//std::cout << "Pressed Mouse button LEFT!" << std::endl;
			}
			if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
				//std::cout << "Pressed Mouse button RIGHT!" << std::endl;
			}
		}

		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)  //friend definition
		{
			Window* win =  (Window*) glfwGetWindowUserPointer(window);
			//std::cout << xpos << " " << ypos << std::endl;
			win->mousePosition.x = xpos;
			win->mousePosition.y = ypos;
		}

		bool Window::init()
		{	
			if (!glfwInit()) {

				std::cout << "Failed to initialize GLFW." << std::endl;
				return false;
			}

			m_Window = glfwCreateWindow(m_Width, m_Height , m_Title , NULL, NULL);

			if (!m_Window) {
				glfwTerminate();
				std::cout << "Failed to create GLFW Window." << std::endl;
			}
			glfwMakeContextCurrent(m_Window);

			if (glewInit() != GLEW_OK)
			{
				std::cout << "Could not initialize GLEW!" << std::endl;
				return false;
			}


			glfwSetKeyCallback(m_Window, key_callback);
			glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, cursor_position_callback);


			glfwSetWindowUserPointer(m_Window, this);// need this to access the currently active window. This is very important.
			glfwSwapInterval(0.0f);// Vsync

			std::cout << "OpenGl" << glGetString(GL_VERSION) << std::endl;
			return true;
		}

		void Window::clear() const
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		
		void Window::update()
		{
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) //shitty error handling. Improve this later.
				std::cout << "OpenGL ERROR: " << error << std::endl;


			glfwPollEvents();
			glfwSwapBuffers(m_Window);
			glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
			glViewport(0, 0, m_Width, m_Height);
		}

		bool Window::closed() const
		{
			return glfwWindowShouldClose(m_Window);
		}

		void Window::drawRightAngledTriangle()
		{
			glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f( 0.0f,  0.5f);
			glVertex2f( 0.5f, -0.5f);
			glEnd();
		}
	}
}