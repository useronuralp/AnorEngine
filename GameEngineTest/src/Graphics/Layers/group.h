#pragma once
#include "pch.h"
#include "../renderable2D.h"

namespace GameEngineTest {
	namespace Graphics {

		class ENGINE_API Group : public Renderable2D
		{

		private:
			std::vector<Renderable2D*> m_Renderables;
			Math::mat4 m_TransformationMatrix;
		};

	}
}