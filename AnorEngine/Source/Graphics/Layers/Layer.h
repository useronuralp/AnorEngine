#pragma once
namespace AnorEngine {
	//Forward declerations
	namespace Input
	{
		class Event;
	}
	namespace Graphics
	{
		class Camera;
		class ANOR_API Layer
		{
		protected:
			Layer();
		public:
			virtual ~Layer();
			Layer(const char* name);
			virtual void OnAttach() {};
			virtual void OnUpdate(bool IsRuntime = false, float deltaTime = 1.0f) {};
			virtual void OnEvent(Ref<Input::Event> event) {};
			virtual void OnResizeViewport(uint32_t width, uint32_t height) {};
			virtual void OnMouseScroll(float xoffset, float yoffset) {};
			virtual void OnImGuiRender() {};
			inline const char* GetName() { return m_LayerName; }
			void logInfoDebug();
		protected:
			const char* m_LayerName;
		};
	}
}