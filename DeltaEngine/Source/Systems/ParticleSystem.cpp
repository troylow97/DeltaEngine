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
          for (unsigned int i = 0; i < count; ++i)
          {
            if (ps.m_ActiveParticles >= ps.maxParticles ||
              FindInactiveParticle() >= ps.m_ParticlePool.size())
              break;
            Particle& particle = ps.m_ParticlePool[FindInactiveParticle()];
            ++ps.m_ActiveParticles;
            particle.active = true;

            float disp = 0.5f;
            switch (ps.genType)
            {
            case ParticleEmitter::GenType::Random:
              disp = Random::RandomFloat();
              break;
            case ParticleEmitter::GenType::Loop:
              disp = 1.f * ps.durationTimer / ps.duration;
              break;
            case ParticleEmitter::GenType::PingPong:
              disp = 1.f * ps.durationTimer / ps.duration;
              break;
            case ParticleEmitter::GenType::Spread:
              disp = 1.f * i / count;
              break;
            }
            disp = (disp * 2 - 1);
            switch (ps.shape)
            {
            case ParticleEmitter::Shape::None:
              particle.transform.position = Vector3();
              particle.velocity = Vector3::up() + Vector3::right() * (2 * Random::RandomFloat() - 1);
              break;
            case ParticleEmitter::Shape::Circle:
              particle.transform.position = Vector3(
                cosf(disp * Math::pi) * ps.radius,
                sinf(disp * Math::pi) * ps.radius);
              particle.velocity = Vector3(
                cosf(disp * Math::pi),
                sinf(disp * Math::pi));
              break;
            case ParticleEmitter::Shape::Line:
              particle.transform.position = Vector3(
                ps.radius * disp,
                0);
              particle.velocity = Vector3::up() + Vector3::right() * (2 * Random::RandomFloat() - 1);
              break;
            case ParticleEmitter::Shape::Box:
              particle.transform.position = Vector3();
              particle.velocity = Vector3::up() + Vector3::right() * (2 * Random::RandomFloat() - 1);
              break;
            }
            particle.transform.scale = Vector3(
              Random::RandomFloatRange(ps.startSizeMin.x, ps.startSizeMax.x),
              Random::RandomFloatRange(ps.startSizeMin.y, ps.startSizeMax.y),
              Random::RandomFloatRange(ps.startSizeMin.z, ps.startSizeMax.z)
            );
            particle.transform.rotation = Quaternion::Identity();

            if (ps.startLifetimeMin > ps.startLifetimeMax)
              ps.startLifetimeMax = ps.startLifetimeMin;
            particle.lifeTime = Random::RandomFloatRange(ps.startLifetimeMin, ps.startLifetimeMax);
            particle.lifeTimer = 0;
          }
        };

        for (auto burst : ps.bursts)
          if (ps.durationTimer <  burst.time &&
            ps.durationTimer + static_cast<float>(FixedDeltaTime()) > burst.time)
            Emit(burst.count);

        ps.emissionTimer += static_cast<float>(FixedDeltaTime());
        ps.durationTimer += static_cast<float>(FixedDeltaTime());
        ps.playbackTimer += static_cast<float>(FixedDeltaTime());
        ps.rateOverTime = Math::Clamp(ps.rateOverTime, 0, 100);

        while (ps.emissionTimer > 1.0f / ps.rateOverTime)
        {
          ps.emissionTimer -= 1.0f / ps.rateOverTime;
          Emit(1);
        }
        if (ps.durationTimer > ps.duration)
          ps.durationTimer -= ps.duration;

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
            std::cerr << particle.lifeTimer << "," << particle.lifeTime << std::endl;
            particle.active = false;
            --ps.m_ActiveParticles;
            ++index;
            continue;
          }

          particle.lifeTimer += static_cast<float>(FixedDeltaTime());
          particle.transform.position += particle.velocity * static_cast<float>(FixedDeltaTime());
          particle.transform.scale = Vector3(
            Random::RandomFloatRange(
              ps.sizeOverLifetimeXMin.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.sizeOverLifetimeXMax.Evaluate(particle.lifeTimer / particle.lifeTime)),
            Random::RandomFloatRange(
              ps.sizeOverLifetimeYMin.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.sizeOverLifetimeYMax.Evaluate(particle.lifeTimer / particle.lifeTime)),
            Random::RandomFloatRange(
              ps.sizeOverLifetimeZMin.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.sizeOverLifetimeZMax.Evaluate(particle.lifeTimer / particle.lifeTime))
            );
          //particle.transform.scale = Vector3::one();
          std::cerr
            << particle.lifeTimer / particle.lifeTime << ","
            << ps.sizeOverLifetimeXMin.Evaluate(particle.lifeTimer / particle.lifeTime) << ","
            << ps.sizeOverLifetimeXMin.Evaluate(0.5f) << ","
            << std::endl;

          particle.transform.rotation = Quaternion::AngleAxis(particle.lifeTimer * index / 50, Vector3::forward());

          ++index;
        }

      });
  }

  void ParticleSystem::LateUpdate()
  {

  }
}
