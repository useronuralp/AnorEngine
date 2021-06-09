#include "pch.h"
#include "ParticleSystem.h"
namespace AnorEngine
{
	namespace Graphics
	{
		Particle::Particle()
		{
			BufferLayout Layout = { {ShaderDataType::vec3, "a_Position", 0} };
			m_LifeStartTime = std::chrono::high_resolution_clock::now(); //TODO: set the lifetime start when you render the particle for the first time.
			m_IsDead = false;
			m_Transform = glm::mat4(1.0f);
			m_ParticleVAO = std::make_shared<VertexArray>();
			m_ParticleVAO->AddVertexBuffer(std::make_shared<Buffer>(m_Vertices, 12 * sizeof(float), Layout));
			m_ParticleVAO->SetIndexBuffer(std::make_shared<IndexBuffer>(m_Indices, 6));
			m_ParticleShader = ShaderLibrary::GetShader("2DShader");
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
		void Particle::RandomizeParticle()
		{
			m_Speed -= Random::GetFloat();
			m_Size -= Random::GetFloat() * m_Size;
			m_Size = std::max(m_Size, 0.05f);
			m_Transform = glm::scale(m_Transform, { m_Size, m_Size, 0 });
			m_MoveDirection.x += Random::GetFloat() * 2.0 - 1.0;
			m_MoveDirection.y += Random::GetFloat() * 2.0 - 1.0;
			m_Color.x -= Random::GetFloat();
		}
		ParticleSystem::ParticleSystem(ParticleProperties properties)
			:m_Properties(properties) {}

		void ParticleSystem::CreateParticles(int count)
		{
			for (int i = 0; i < count; i++)
			{
				Ref<Particle> particle = std::make_shared<Particle>();
				particle->m_Color = m_Properties.Color;
				particle->m_LifeTime = m_Properties.LifeTime;
				particle->m_MoveDirection = m_Properties.MoveDirection;
				particle->m_Size = m_Properties.Size;
				particle->m_Speed = m_Properties.Speed;
				particle->m_InitialSpeed = m_Properties.Speed;
				particle->m_StartingAlphaValue = m_Properties.Color.a;
				particle->m_Transform = glm::translate(particle->m_Transform, m_StartPosition);
				particle->RandomizeParticle();
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