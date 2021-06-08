#include "pch.h"
#include "ParticleSystem.h"
namespace AnorEngine
{
	namespace Graphics
	{
		Particle::Particle()
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} };
			m_LifeStartTime = std::chrono::high_resolution_clock::now();
			m_Speed = 20.0f;
			m_InitialSpeed = m_Speed;
			m_LifeTime = 5;
			m_Size = 0.3f;
			m_MoveDirection = { 0.0f,0.0f,0.0f };
			m_Color = { 1, 1, 1, 0.5f };
			m_IsDead = false;
			m_StartingAlphaValue = m_Color.a;
			m_Transform = glm::mat4(1.0f);
			m_ParticleVAO = std::make_shared<VertexArray>();
			m_ParticleVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 12 * sizeof(float), Layout));
			m_ParticleVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 6));
			m_ParticleShader = ShaderLibrary::GetShader("2DShader");
			//Randomization part----------------------------------------------------------------------------------------
			m_Speed -= Random::GetFloat();
			m_Size -= Random::GetFloat() * m_Size;
			m_Size = std::max(m_Size, 0.05f);
			m_Transform = glm::scale(m_Transform, { m_Size, m_Size, 0 });
			m_MoveDirection.x += Random::GetFloat() * 2.0 - 1.0;
			m_MoveDirection.y += Random::GetFloat() * 2.0 - 1.0;
			m_Color.y -= Random::GetFloat();
		}
		void Particle::OnUpdate(float deltaTime)
		{
			auto currentTime = std::chrono::high_resolution_clock::now();
			float elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_LifeStartTime).count();

			if (elapsedTime >= m_LifeTime)
				m_IsDead = true;
			m_Color.a -= (m_StartingAlphaValue / m_LifeTime) * elapsedTime * deltaTime;
			Renderer2D::DrawPrimitive(m_ParticleVAO, m_ParticleShader, m_Transform, m_Color);
			m_Transform = glm::translate(m_Transform, { m_MoveDirection.x * (m_Speed * deltaTime), m_MoveDirection.y * (m_Speed * deltaTime), 0 });
			m_Speed -= (elapsedTime / m_LifeTime) * m_InitialSpeed * deltaTime;
			//m_Transform = glm::rotate(m_Transform, 8.0f * deltaTime, { 0,0,0.5f });
		}

		void ParticleSystem::CreateParticles(int count)
		{
			for (int i = 0; i < count; i++)
			{
				Ref<Particle> particle = std::make_shared<Particle>();
				particle->m_Transform = glm::translate(particle->m_Transform, m_StartPosition);
				m_Particles.push_back(particle);
			}
		}
		void ParticleSystem::OnUpdate(float deltaTime)
		{
			for (int i = 0; i < m_Particles.size(); i++)
			{
				if (m_Particles[i]->m_IsDead)
				{
					m_Particles.erase(m_Particles.begin() + i);
					continue;
				}
				m_Particles[i]->OnUpdate(deltaTime);
			}
		}
	}
}