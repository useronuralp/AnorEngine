#pragma once
#include <Events/Event.h>
namespace AnorEngine {
	namespace Graphics
	{
		class ANOR_API Layer
		{
		protected:
			Layer();
		public:
			virtual ~Layer();
			Layer(const char* name);
			virtual void OnAttach() {};
			virtual void OnUpdate(float deltaTime = 1.0f) {};
			virtual void OnEvent(Ref<Input::Event> event) {};
			virtual void OnImGuiRender() {};
			inline const char* getName() { return m_LayerName; }
			void logInfoDebug();
		protected:
			const char* m_LayerName;
		private:
		};
	}
}