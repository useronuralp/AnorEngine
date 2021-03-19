#pragma once

namespace GameEngineTest {
	namespace Graphics
	{
		class ENGINE_API Layer
		{

		protected:
			Layer();
		public:
			Layer(const char* name);
			virtual void OnAttach();
			virtual void OnUpdate(); 	
			inline const char* getName() { return m_LayerName; }
		protected:
			const char* m_LayerName;
		};
	}
}