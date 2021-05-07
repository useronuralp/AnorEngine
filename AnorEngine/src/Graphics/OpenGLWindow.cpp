#include "pch.h"
#include "OpenGLWindow.h"
#include "../Events/EventHandler.h"
namespace AnorEngine {
	namespace Graphics {

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{	
			if (action == GLFW_PRESS || action == GLFW_REPEAT)
				Input::EventHandler::SubmitEvent(new Input::KeyPressEvent(key, action));

			else if (action == GLFW_RELEASE)
				Input::EventHandler::SubmitEvent(new Input::KeyReleaseEvent(key));

		}
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			OpenGLWindow* win = (OpenGLWindow*)glfwGetWindowUserPointer(window);
			if(action == GLFW_PRESS)
				Input::EventHandler::SubmitEvent(new Input::MouseClickEvent(button, action));
			else
				Input::EventHandler::SubmitEvent(new Input::MouseReleaseEvent(button, action));
		}
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			OpenGLWindow* win = (OpenGLWindow*)glfwGetWindowUserPointer(window);
			Input::EventHandler::SubmitEvent(new Input::MouseMoveEvent(xpos, ypos));
		}
		void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{
			//OpenGLWindow* win = (OpenGLWindow*)glfwGetWindowUserPointer(window);
		}
		void APIENTRY openglErrorCallbackFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			using namespace std;
			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
			cout << "---------------------opengl-callback-start------------" << endl;
			cout << "message: " << message << endl;
			cout << "type: ";
			switch (type) {
			case GL_DEBUG_TYPE_ERROR:
				cout << "ERROR";
				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
				cout << "DEPRECATED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
				cout << "UNDEFINED_BEHAVIOR";
				break;
			case GL_DEBUG_TYPE_PORTABILITY:
				cout << "PORTABILITY";
				break;
			case GL_DEBUG_TYPE_PERFORMANCE:
				cout << "PERFORMANCE";
				break;
			case GL_DEBUG_TYPE_OTHER:
				cout << "OTHER";
				break;
			}
			cout << endl;

			cout << "id: " << id << endl;
			cout << "severity: ";
			switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:
				cout << "LOW";
				break;
			case GL_DEBUG_SEVERITY_MEDIUM:
				cout << "MEDIUM";
				break;
			case GL_DEBUG_SEVERITY_HIGH:
				cout << "HIGH";
				break;
			}
			cout << endl;
			cout << "---------------------opengl-callback-end--------------" << endl;
		}
		OpenGLWindow::OpenGLWindow(const char* title, int width, int height) 
		{	
			
			m_Title = title;
			m_Height = height;
			m_Width = width;
	
			if (!init())
			{ // this is how you call init function. Inside an if statement. It is a little bit tricky.
				glfwTerminate();
			}
		}
		OpenGLWindow::~OpenGLWindow() 
		{	
			glfwTerminate();
		}

		bool OpenGLWindow::init()
		{		
			if (!glfwInit()) //calling glfwInit() inside this if statement.
			{
				std::cout << "Failed to initialize GLFW." << std::endl;
				return false;
			}
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
			m_Window = glfwCreateWindow(m_Width, m_Height , m_Title , NULL, NULL);
			int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				// initialize debug output 
			}
			if (!m_Window) {
				glfwTerminate();
				std::cout << "Failed to create GLFW OpenGLWindow." << std::endl;
			}

			glfwMakeContextCurrent(m_Window);

			if (glewInit() != GLEW_OK)
			{
				std::cout << "Could not initialize GLEW!" << std::endl;
				return false;
			}
			
			glDebugMessageCallback(openglErrorCallbackFunction, nullptr);
			glfwSetKeyCallback(m_Window, key_callback);
			glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
			glfwSetCursorPosCallback(m_Window, cursor_position_callback);
			glfwSetScrollCallback(m_Window, mouse_scroll_callback);

			glfwSetWindowUserPointer(m_Window, this);// need this to access the currently active window. This is very important.
			glfwSwapInterval(0);// Vsync

			std::cout << "OpenGL" << glGetString(GL_VERSION) << std::endl;
			return true;
		}

		void OpenGLWindow::Clear() const
		{	
			glClearColor(1 - 0.95, 1 - 0.95, 1 - 0.95, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		
		void OpenGLWindow::Update()
		{
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) //shitty error handling. Improve this later.
				std::cout << "OpenGL ERROR: " << error << std::endl;

			m_Time = glfwGetTime();
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
			glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
			glViewport(0, 0, m_Width, m_Height);
		}

		bool OpenGLWindow::IsClosed() const
		{
			return glfwWindowShouldClose(m_Window); // returns the close flag of the window. If either the 'x' button is clicked or the alt+f4 is pressed, this function will return 1 
			//indicating that the window should be closed.
		}

		void OpenGLWindow::drawRightAngledTriangle()
		{	
			//Legacy OpenGL, this is an old way of drawing stuff on the screen.
			glBegin(GL_TRIANGLES);
			glVertex2f(-0.5f, -0.5f);
			glVertex2f( 0.0f,  0.5f);
			glVertex2f( 0.5f, -0.5f);
			glEnd();
		}
	}
}