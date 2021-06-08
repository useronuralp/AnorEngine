#pragma once
#include "Buffers/VertexArray.h"
#include "Shader.h"
#include "Utility/Randomizer.h"
#include "Renderer/Renderer2D.h"
namespace AnorEngine
{
	namespace Graphics
	{
		struct ANOR_API Particle
		{		
			float m_Vertices[3 * 4] =
			{
				-0.5f, -0.5f, 0.0f,
				 0.5f, -0.5f, 0.0f,
				 0.5f,  0.5f, 0.0f,
				-0.5f,  0.5f, 0.0f
			};
			uint32_t m_Indices[6] =
			{
				0, 1, 2, 2, 3, 0
			};
			float								  m_Speed;
			float								  m_LifeTime;
			float								  m_Size;
			float								  m_StartingAlphaValue;
			float								  m_InitialSpeed;
			bool								  m_IsDead;
			Ref<VertexArray>					  m_ParticleVAO;
			Ref<Shader>							  m_ParticleShader;
			glm::mat4							  m_Transform;
			glm::vec3							  m_MoveDirection;
			glm::vec4							  m_Color;
			std::chrono::steady_clock::time_point m_LifeStartTime;
		public:
			Particle();
		public:
			void OnUpdate(float deltaTime);
		};

		class ANOR_API ParticleSystem
		{
			glm::vec3 m_StartPosition = { 0.0f, 0.0f, 0.0f };
			std::vector<Ref<Particle>> m_Particles;
		public:
			void CreateParticles(int count);
			void OnUpdate(float deltaTime);
			//void OnImGui();
		};
	}
}