#pragma once
#include "Entity.h"
namespace AnorEngine
{
	class ANOR_API ScriptableEntity
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	private:
		Entity m_Entity;
		friend class Scene;
	};
}