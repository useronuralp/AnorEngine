#pragma once 
//Forward declerations
class GLFWwindow;

namespace AnorEngine {
	namespace Graphics {

		class ANOR_API OpenGLWindow{
		private:
			GLFWwindow* m_Window = nullptr;
			const char *m_Title;
			int m_Width, m_Height;
			bool m_Closed;
			double m_Time = 0.0f;
		public:
			OpenGLWindow(const char* name, int width, int height);
			~OpenGLWindow();
			void Clear() const;
			void Close();
			void Update();
			bool IsClosed() const;
			inline int GetHeight() { return m_Height; }
			inline int GetWidth() { return m_Width; }
			inline GLFWwindow* GetNativeWindow() { return m_Window; }
			inline double  GetRenderTime() { return m_Time; }
			inline void SetInputMode(int Mode, int Value);
			inline int GetInputMode(int Mode);
			void GetWindowSize(int* width, int* height);
		private:
			bool init();
		private:
			friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
			friend void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
			friend void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
			friend void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
			friend void window_resize_callback(GLFWwindow* window, int width, int height);
		};
	}
}
