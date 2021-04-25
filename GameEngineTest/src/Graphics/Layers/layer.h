#pragma once

namespace GameEngineTest {
	namespace Graphics
	{
		class ENGINE_API Layer
		{

		protected:
			Layer();
		public:
			virtual ~Layer();
		public:
			Layer(const char* name);
			virtual void OnAttach() {};
			virtual void OnUpdate() {};
			virtual void OnEvent() {};
			virtual bool OnMouseLeftClickPressedEvent() = 0;
			virtual bool OnMouseLeftClickReleasedEvent() = 0;
			virtual bool OnMouseMove(float xpos, float ypos) = 0;
			inline const char* getName() { return m_LayerName; }
			void logInfoDebug();
		protected:
			const char* m_LayerName;
		private:
		};
	}
}