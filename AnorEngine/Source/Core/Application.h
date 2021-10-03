#pragma once
namespace AnorEngine
{	
	//Forward declarataions
	namespace Graphics
	{
		class OpenGLWindow;
	}
	//Forward declarataions
	namespace Input
	{
		class Event; 
	}
	class ANOR_API Application
	{
	protected:
		Ref<Graphics::OpenGLWindow> m_OpenGLWindow;
	private:
		static Application* s_Instance; //static singleton instance of this class.
	public:
		Application(const char* appName = "Anor Engine");
		virtual ~Application();
	public:
		virtual void OnImGuiRender() {};
		virtual void OnEvent(Ref<Input::Event> event) {};
		virtual void Close();
		virtual void Run();
	public:
		inline static Application& Get() { return *s_Instance; }
		inline Ref<Graphics::OpenGLWindow> GetOpenGLWindow() { return m_OpenGLWindow; }
	protected:
		virtual void LogInfoDebug();
	};
}

