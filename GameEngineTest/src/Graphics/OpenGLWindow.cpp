#include "pch.h"
#include "OpenGLWindow.h"

namespace GameEngineTest {
	namespace Graphics {

		void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			OpenGLWindow* win = (OpenGLWindow*)glfwGetWindowUserPointer(window);
			if (key == ONURALP_KEY_LEFT_CONTROL && action == GLFW_PRESS)
			{
				if (!win->isMouseCaptured)
				{
					glfwSetInputMode(win->m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					win->isMouseCaptured = true;
					win->ImGui->wantToCaptureMouse = false;
				}
				else
				{
					glfwSetInputMode(win->m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					win->isMouseCaptured = false;
					win->ImGui->wantToCaptureMouse = true;
					win->isInitialMouseCaptured = false;
					win->m_Keys['W'] = false;
					win->m_Keys['A'] = false;
					win->m_Keys['S'] = false;
					win->m_Keys['D'] = false;
				}
			}
			if (win->isMouseCaptured)
			{
				if (key == ONURALP_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT))
				{
					win->m_Keys['W'] = true;
				}
				else if ((key == ONURALP_KEY_W && (action == GLFW_RELEASE)))
				{
					win->m_Keys['W'] = false;
				}
				if (key == ONURALP_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT))
				{
					win->m_Keys['S'] = true;
				}
				else if (key == ONURALP_KEY_S && (action == GLFW_RELEASE || action == GLFW_REPEAT))
				{
					win->m_Keys['S'] = false;
				}
				if (key == ONURALP_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT))
				{
					win->m_Keys['A'] = true;
				}
				else if ((key == ONURALP_KEY_A && (action == GLFW_RELEASE)))
				{
					win->m_Keys['A'] = false;
				}
				if (key == ONURALP_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT))
				{
					win->m_Keys['D'] = true;
				}
				else if ((key == ONURALP_KEY_D && (action == GLFW_RELEASE)))
				{
					win->m_Keys['D'] = false;
				}
			}
		}
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
		{
			OpenGLWindow* win =  (OpenGLWindow*) glfwGetWindowUserPointer(window);
			//std::cout << "Button clicked:" << button << std::endl;
		}
		void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
		{
			OpenGLWindow* win = (OpenGLWindow*)glfwGetWindowUserPointer(window);
			if (glfwGetInputMode(win->m_Window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
			{
				if (!win->isInitialMouseCaptured)
				{
					win->lastX = (float)xpos;
					win->lastY = (float)ypos;
					win->isInitialMouseCaptured = true;
				}
				float xoffset = (float)(xpos - win->lastX);
				float yoffset = (float)(win->lastY - ypos);
				win->lastX = (float)xpos;
				win->lastY = (float)ypos;


				xoffset *= win->sensitivity;
				yoffset *= win->sensitivity;

				win->YAW += xoffset;
				win->PITCH += yoffset;

				if (win->PITCH > 89.0f)
					win->PITCH = 89.0f;
				if (win->PITCH < -89.0f)
					win->PITCH = -89.0f;

				glm::vec3 direction;
				direction.x = cos(glm::radians(win->YAW)) * cos(glm::radians(win->PITCH));
				direction.y = sin(glm::radians(win->PITCH));
				direction.z = sin(glm::radians(win->YAW)) * cos(glm::radians(win->PITCH));
				win->cameraFront = glm::normalize(direction);

				win->mousePosition.x = xpos;
				win->mousePosition.y = ypos;
			}
		}
		void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
		{	
			//OpenGLWindow* win = (OpenGLWindow*)glfwGetWindowUserPointer(window);
		}
		void APIENTRY openglErrorCallbackFunction(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam)
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
			ImGui = nullptr;
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

		void OpenGLWindow::clear() const
		{	
			glClearColor(1 - 0.95, 1 - 0.95, 1 - 0.95, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		
		void OpenGLWindow::update()
		{
			GLenum error = glGetError();
			if (error != GL_NO_ERROR) //shitty error handling. Improve this later.
				std::cout << "OpenGL ERROR: " << error << std::endl;

			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame; //delta time implementation. You could abstract this.
			lastFrame = currentFrame;

			cameraSpeed = 3 * deltaTime; //adjust speed here.
			for (auto key : m_Keys)
			{
				if (key.second)
				{
					switch (key.first)
					{
						case 'W':
							cameraPos += cameraSpeed * cameraFront;
							break;
						case 'S':
							cameraPos -= cameraSpeed * cameraFront;
							break;
						case 'A':
							cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
							break;
						case 'D':
							cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
							break;
					}
				}
			}
			glfwPollEvents();
			glfwSwapBuffers(m_Window);
			glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
			glViewport(0, 0, m_Width, m_Height);
		}

		bool OpenGLWindow::isClosed() const
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