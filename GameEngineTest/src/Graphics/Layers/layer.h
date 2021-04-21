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
			virtual void OnAttach();
			virtual void OnUpdate(); 	
			virtual void OnEvent();
			inline const char* getName() { return m_LayerName; }
			void logInfoDebug();
		protected:
			const char* m_LayerName;
		private:
		};
	}
}