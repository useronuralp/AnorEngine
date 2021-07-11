#pragma once
#include "Entity.h"
namespace AnorEngine
{
	class ANOR_API ScriptableEntity
	{
	public:
		ScriptableEntity() = default;
		ScriptableEntity(const ScriptableEntity&) = default;
		virtual ~ScriptableEntity() {};
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Entity.GetComponent<T>();
		}
	protected:
		virtual void OnUpdate(float deltaTime) {};
		virtual void OnDestroy() {};
		virtual void OnCreate() {};
	private:
		Entity m_Entity;
		friend class Scene;
	};
}