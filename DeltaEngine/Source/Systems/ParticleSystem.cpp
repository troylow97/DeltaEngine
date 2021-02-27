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
        if (!ParticleEmitter::particlePools.count(id))
          ParticleEmitter::particlePools[id] = std::vector<ParticleEmitter::Particle>();
        ParticleEmitter::particlePools[id].resize(ps.maxParticles);

        auto Emit = [&id, &ps](unsigned int count)
        {
          auto FindInactiveParticle = [&id, &ps]()
          {
            for (unsigned int i = 0; i < ps.maxParticles; ++i)
            {
              if (!ps.particlePools[id][i].active)
                return i;
            }
            return ps.maxParticles;
          };
          using Particle = ParticleEmitter::Particle;
          for (unsigned int i = 0; i < count; ++i)
          {
            if (ps.m_ActiveParticles >= ps.maxParticles ||
              FindInactiveParticle() >= ps.particlePools[id].size())
              break;
            Particle& particle = ps.particlePools[id][FindInactiveParticle()];
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
              particle.velocity = Vector3::up();
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
                ps.shapeTransform.position.x + (ps.radius + ps.shapeTransform.scale.x) * disp,
                ps.shapeTransform.position.y);
              particle.velocity = Vector3::up();
              break;
            case ParticleEmitter::Shape::Box:
              particle.transform.position = Vector3(
                Random::RandomFloatRange(
                  ps.shapeTransform.position.x - ps.shapeTransform.scale.x,
                  ps.shapeTransform.position.x + ps.shapeTransform.scale.x),
                Random::RandomFloatRange(
                  ps.shapeTransform.position.y - ps.shapeTransform.scale.y,
                  ps.shapeTransform.position.y + ps.shapeTransform.scale.y),
                Random::RandomFloatRange(
                  ps.shapeTransform.position.z - ps.shapeTransform.scale.z,
                  ps.shapeTransform.position.z + ps.shapeTransform.scale.z)
              );
              particle.velocity = Vector3::up();
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

        //for (auto burst : ps.bursts)
        //  if (ps.durationTimer <  burst.time &&
        //    ps.durationTimer + static_cast<float>(FixedDeltaTime()) > burst.time)
        //    Emit(burst.count);

        ps.emissionTimer += static_cast<float>(FixedDeltaTime());
        ps.durationTimer += static_cast<float>(FixedDeltaTime());
        ps.rateOverTime = Math::Clamp(ps.rateOverTime, 0, 100);

        while (ps.emissionTimer > 1.0f / ps.rateOverTime)
        {
          ps.emissionTimer -= 1.0f / ps.rateOverTime;
          Emit(1);
        }
        if (ps.durationTimer > ps.duration)
          ps.durationTimer -= ps.duration;

        int index = 0;
        for (auto& particle : ps.particlePools[id])
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

          particle.transform.position += particle.velocity * static_cast<float>(DeltaTime());

          switch (ps.sizeOverLifetime.type)
          {
          case BezierCurve::Type::Constant:
            particle.transform.scale = Vector3(
              ps.sizeOverLifetime.minX.Evaluate(0),
              ps.sizeOverLifetime.minY.Evaluate(0),
              ps.sizeOverLifetime.minZ.Evaluate(0)
            );
            break;
          case BezierCurve::Type::ConstantCurve:
            particle.transform.scale = Vector3(
              ps.sizeOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.sizeOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.sizeOverLifetime.minZ.Evaluate(particle.lifeTimer / particle.lifeTime)
              );
            break;
          case BezierCurve::Type::RandomBetweenConstants:
            particle.transform.scale = Vector3(
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minX.Evaluate(0),
                ps.sizeOverLifetime.maxX.Evaluate(0)),
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minY.Evaluate(0),
                ps.sizeOverLifetime.maxY.Evaluate(0)),
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minZ.Evaluate(0),
                ps.sizeOverLifetime.maxZ.Evaluate(0))
            );
            break;
          case BezierCurve::Type::RandomBetweenCurves:
            particle.transform.scale = Vector3(
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.sizeOverLifetime.maxX.Evaluate(particle.lifeTimer / particle.lifeTime)),
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.sizeOverLifetime.maxY.Evaluate(particle.lifeTimer / particle.lifeTime)),
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minZ.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.sizeOverLifetime.maxZ.Evaluate(particle.lifeTimer / particle.lifeTime))
            );
          }

          particle.transform.rotation = Quaternion::AngleAxis(particle.lifeTimer * index / 50, Vector3::forward());

          particle.velocity = Vector3(
            Random::RandomFloatRange(
              ps.velocityOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.velocityOverLifetime.maxX.Evaluate(particle.lifeTimer / particle.lifeTime)),
            Random::RandomFloatRange(
              ps.velocityOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.velocityOverLifetime.maxY.Evaluate(particle.lifeTimer / particle.lifeTime)),
            Random::RandomFloatRange(
              ps.velocityOverLifetime.minZ.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.velocityOverLifetime.maxZ.Evaluate(particle.lifeTimer / particle.lifeTime))
          );

          Color min = ps.colorOverLifetime.min.Evaluate(particle.lifeTimer / particle.lifeTime);
          Color max = ps.colorOverLifetime.max.Evaluate(particle.lifeTimer / particle.lifeTime);

          switch (ps.colorOverLifetime.type)
          {
          case Gradient::Type::ConstantColor:
          case Gradient::Type::ConstantGradient:
            particle.color = Color(min);
            break;
          case Gradient::Type::RandomBetweenColors:
          case Gradient::Type::RandomBetweenGradients:
            particle.color = Color(
              Random::RandomFloatRange(min.r, max.r),
              Random::RandomFloatRange(min.g, max.g),
              Random::RandomFloatRange(min.b, max.b),
              Random::RandomFloatRange(min.a, max.a)
            );
            break;
          }

          for (int f = 0; f <= 10; f += 1)
          {
            std::cerr << f << ": " << ps.sizeOverLifetime.minX.Evaluate(f / 10.0f) << std::endl;
          }

          particle.lifeTimer += static_cast<float>(DeltaTime());
          ++index;
        }
      });
  }

  void ParticleSystem::LateUpdate()
  {

  }
}
