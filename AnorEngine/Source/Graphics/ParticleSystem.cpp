#include "pch.h"
#include "ParticleSystem.h"
namespace AnorEngine
{
	namespace Graphics
	{
		Particle::Particle()
		{
			m_IsDead = false;
			m_IsFirstRender = true;
			m_Position = { 0.0f,0.0f,0.0f };
			m_MoveDirection = { 0.0f,0.0f,0.0f };
		}
		void Particle::OnUpdate(float deltaTime)
		{
			if (m_IsFirstRender)
			{
				m_LifeStartTime = std::chrono::system_clock::now();
				m_IsFirstRender = false;
			}
			auto currentTime = std::chrono::system_clock::now();
			long float elapsedTime = (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - m_LifeStartTime).count()) / 1000;

			if (elapsedTime >= m_LifeTime)
			{
				m_IsDead = true;
				return;
			}

			Renderer2D::Submit(m_Position, { m_Size , m_Size }, m_Color);
			m_Color.a -= (elapsedTime / m_LifeTime) * (m_Color.a / pow(m_StartingAlphaValue, 3)) * deltaTime;
			m_Position += glm::vec3(m_MoveDirection.x * (m_Speed* deltaTime), m_MoveDirection.y * (m_Speed* deltaTime), 0.0f);
			m_Speed -= (elapsedTime / m_LifeTime) * (m_Speed * m_InitialSpeed) * deltaTime;
			//m_Transform = glm::rotate(m_Transform, 8.0f * deltaTime, { 0,0,0.5f });
		}
		void Particle::RandomizeParticle()
		{
			m_Speed -= Random::GetFloat() * 10.0f;
			m_InitialSpeed = m_Speed;
			m_Size -= Random::GetFloat() * m_Size;
			m_Size = std::max(m_Size, 0.05f);
			//m_Transform = glm::scale(m_Transform, { m_Size, m_Size, 0 });
			m_MoveDirection.x += Random::GetFloat() * 2.0 - 1.0;
			m_MoveDirection.y += Random::GetFloat() * 2.0 - 1.0;
			m_Color.x -= Random::GetFloat();
		}
		ParticleSystem::ParticleSystem(ParticleProperties properties)
			:m_Properties(properties)
		{
			m_EmissionPoint = m_Properties.EmissionPoint;
		}

		void ParticleSystem::CreateParticles(int count)
		{
			for (int i = 0; i < count; i++)
			{
				Ref<Particle> particle = std::make_shared<Particle>();
				particle->m_Color = m_Properties.Color;
				particle->m_LifeTime = m_Properties.LifeTime;
				particle->m_Size = m_Properties.Size;
				particle->m_Speed = m_Properties.Speed;
				particle->m_InitialSpeed = m_Properties.Speed;
				particle->m_StartingAlphaValue = m_Properties.Color.a;
				particle->m_Position.x += m_EmissionPoint.x;
				particle->m_Position.y += m_EmissionPoint.y;
				particle->m_Position.z = 0.0f;
				particle->RandomizeParticle();
				m_Particles.push_back(particle);
			}
		}
		void ParticleSystem::OnUpdate(const float& deltaTime)
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
		void ParticleSystem::SetEmissionPoint(const float& x, const float& y)
		{
			m_EmissionPoint.x = x;
			m_EmissionPoint.y = y;
		}
	}
}