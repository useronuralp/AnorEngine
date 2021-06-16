#pragma once
#include "Buffers/VertexArray.h"
#include "Shader.h"
#include "Utility/Randomizer.h"
#include "Renderer/Renderer2D.h"
namespace AnorEngine
{
	namespace Graphics
	{
		struct ANOR_API ParticleProperties
		{
			float								  Speed;
			float								  LifeTime;
			float								  Size;
			glm::vec3							  StartPosition;
			glm::vec4							  Color;
		};
		class ANOR_API Particle
		{	
			friend class ParticleSystem;
			float										   m_Speed;
			float										   m_LifeTime;
			float										   m_Size;
			float										   m_StartingAlphaValue;
			float										   m_InitialSpeed;
			bool										   m_IsDead;
			bool										   m_IsFirstRender;
			glm::vec3									   m_Position;
			glm::vec3									   m_MoveDirection;
			glm::vec4									   m_Color;
			std::chrono::system_clock::time_point	       m_LifeStartTime;
		public:
			Particle();
		private:
			void RandomizeParticle();
			void OnUpdate(float deltaTime);
		};

		class ANOR_API ParticleSystem
		{
			ParticleProperties m_Properties;
			glm::vec3 m_StartPosition = {0.0f, 0.0f, 0.0f};
			std::vector<Ref<Particle>> m_Particles;
		public:
			ParticleSystem(ParticleProperties properties);
		public:
			void CreateParticles(int count);
			void OnUpdate(float deltaTime);
			//void OnImGui();
		};
	}
}