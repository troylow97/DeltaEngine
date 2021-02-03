/**********************************************************************************
* \file   RenderSystem.cpp
* \brief  The file contains implementation of RenderSystem.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "ParticleSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "ECS/EntityManager.h"
#include "Render/Camera.h"
#include "Render/VideoClip.h"
#include "Render/Mesh.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include "Assets/AssetManager.h"
#include "Core/Utils/Random.h"

namespace DeltaEngine
{
  const unsigned int MAX_PARTICLES_PER_EMITTER = 1024;

  void ParticleSystem::Update()
  {
    em.ForEach(e_query, [&](EntityID id, Transform& tr, ParticleEmitter& ps)
      {
        ps.m_ParticlePool.resize(ps.maxParticles);
        auto Emit = [&ps](unsigned int count)
        {
          auto FindInactiveParticle = [&ps]()
          {
            for (unsigned int i = 0; i < ps.maxParticles; ++i)
            {
              if (!ps.m_ParticlePool[i].active)
                return i;
            }
            return ps.maxParticles;
          };
          using Particle = ParticleEmitter::Particle;
          for (; count > 0; --count)
          {
            if (ps.m_ActiveParticles >= ps.maxParticles ||
              FindInactiveParticle() >= ps.m_ParticlePool.size())
              break;
            Particle& particle = ps.m_ParticlePool[FindInactiveParticle()];
            ++ps.m_ActiveParticles;
            particle.active = true;

            switch (ps.shape)
            {
            case ParticleEmitter::Shape::None:
              particle.transform.position = Vector3();
              break;
            case ParticleEmitter::Shape::Circle:
              particle.transform.position = Vector3();
              break;
            case ParticleEmitter::Shape::Line:
              particle.transform.position = Vector3();
              break;
            case ParticleEmitter::Shape::Box:
              break;
            }
            particle.transform.scale = Vector3::one();
            particle.transform.rotation = Quaternion::Identity();

            // Velocity
            particle.velocity = Vector3::up() + Vector3::right() * (2 * Random::RandomFloat() - 1);
            particle.lifeTime = ps.startLifetime[0];
            particle.lifeTimer = 0;
          }
        };

        ps.durationTimer += static_cast<float>(FixedDeltaTime());
        ps.rateOverTime = Math::Clamp(ps.rateOverTime, 0, 100);

        while (ps.durationTimer > 1.0f / ps.rateOverTime)
        {
          ps.durationTimer -= 1.0f / ps.rateOverTime;
          Emit(1);
        }

        int index = 0;
        for (auto& particle : ps.m_ParticlePool)
        {
          if (!particle.active)
          {
            ++index;
            continue;
          }

          if (particle.lifeTimer >= particle.lifeTime)
          {
            particle.active = false;
            --ps.m_ActiveParticles;
            ++index;
            continue;
          }

          particle.lifeTimer += static_cast<float>(FixedDeltaTime());
          particle.transform.position += particle.velocity * static_cast<float>(FixedDeltaTime());
          particle.transform.scale += -Vector3(1, 1, 1) * static_cast<float>(FixedDeltaTime());

          particle.transform.rotation = Quaternion::AngleAxis(particle.lifeTimer * index / 50, Vector3::forward());

          ++index;
        }

      });
  }

  void ParticleSystem::LateUpdate()
  {

  }
}
