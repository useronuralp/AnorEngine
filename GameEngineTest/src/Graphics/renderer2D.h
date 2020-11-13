#pragma once

#include <GL/glew.h>
#include "../Math/maths.h"
#include "renderable2D.h"
#include "static_sprite.h"

namespace GameEngineTest {
	namespace Graphics {
		
		class Renderer2D 
		{
		protected:
			std::vector<Math::mat4> m_TransformationStack;
			Math::mat4* m_TransformationBack; // used for caching purposes.
		protected:
			Renderer2D()
			{
				m_TransformationStack.push_back(Math::mat4::identity());
				m_TransformationBack = &m_TransformationStack.back();
			}
		public:
			void push(const Math::mat4 &matrix, bool override = false)
			{
				if (override)
					m_TransformationStack.push_back(matrix);
				else
					m_TransformationStack.push_back(m_TransformationStack.back() * matrix);

				m_TransformationBack = &m_TransformationStack.back();
			}
			void pop()
			{
				if (m_TransformationStack.size() > 1)
					m_TransformationStack.pop_back();

				m_TransformationBack = &m_TransformationStack.back();
			}
			virtual void begin() = 0;
			virtual void submit(const Renderable2D* renderable) = 0;
			virtual void flush() = 0;
			virtual void end() = 0;
		};
	}
}