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
#include "Render/Mesh.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include "Assets/AssetManager.h"
#include "Core/Utils/Random.h"

namespace DeltaEngine
{
  const unsigned MAX_PARTICLES_PER_EMITTER = 1024;

  void ParticleSystem::Update()
  {
    em.ForEach(e_query, [&](EntityID id, Transform& tr, ParticleEmitter& ps)
      {
        if (!ParticleEmitter::particlePools.count(id))
          ParticleEmitter::particlePools[id] = std::vector<ParticleEmitter::Particle>();
        ParticleEmitter::particlePools[id].resize(ps.maxParticles);

        auto Emit = [&id, &ps](unsigned count)
        {
          auto FindInactiveParticle = [&id, &ps]()
          {
            for (unsigned i = 0; i < ps.maxParticles; ++i)
            {
              if (!ps.particlePools[id][i].active)
                return i;
            }
            return ps.maxParticles;
          };
          using Particle = ParticleEmitter::Particle;
          for (unsigned i = 0; i < count; ++i)
          {
            if (ps.m_ActiveParticles >= ps.maxParticles ||
              FindInactiveParticle() >= ps.particlePools[id].size())
              break;
            Particle& particle = ps.particlePools[id][FindInactiveParticle()];
            Random::Seed();
            ++ps.m_ActiveParticles;
            particle.active = true;
            particle.seed = Random::RandomUIntRange(0U, ~0U);

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
                ps.shapeTransform.position.x + (ps.radius * ps.shapeTransform.scale.x) * disp,
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
                  ps.shapeTransform.position.z - 1,
                  ps.shapeTransform.position.z + 1)
              );
              particle.velocity = Vector3::up();
              break;
            }
            particle.transform.scale = Vector3(
              Random::RandomFloatRange(ps.startSizeMin.x, ps.startSizeMax.x),
              Random::RandomFloatRange(ps.startSizeMin.y, ps.startSizeMax.y),
              1
            );
            particle.transform.rotation = Quaternion::AngleAxis(
              Random::RandomFloatRange(ps.startRotationMin, ps.startRotationMax),
              Vector3::forward()
            );
            particle.color = Color(
              Random::RandomFloatRange(ps.startColorMin.r, ps.startColorMax.r),
              Random::RandomFloatRange(ps.startColorMin.g, ps.startColorMax.g),
              Random::RandomFloatRange(ps.startColorMin.b, ps.startColorMax.b),
              Random::RandomFloatRange(ps.startColorMin.a, ps.startColorMax.a)
            );

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
        em.ForEach([&](EntityID id, Transform& tr, Camera& c)
          {
            ps.shapeTransform.position = tr.position + Vector2(3.5f, -2.5f);
            ps.shapeTransform.position.z = 0;
          });

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

        // update every particle
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

          Random::Seed(particle.seed);

          switch (ps.velocityOverLifetime.type)
          {
          case BezierCurve::Type::Constant:
            particle.velocity = Vector3(
              ps.velocityOverLifetime.minX.min,
              ps.velocityOverLifetime.minY.min,
              0
            );
            break;
          case BezierCurve::Type::ConstantCurve:
            particle.velocity = Vector3(
              ps.velocityOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.velocityOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
              0
            );
            break;
          case BezierCurve::Type::RandomBetweenConstants:
            particle.velocity = Vector3(
              Random::RandomFloatRange(
                ps.velocityOverLifetime.minX.min,
                ps.velocityOverLifetime.maxX.min),
              Random::RandomFloatRange(
                ps.velocityOverLifetime.minY.min,
                ps.velocityOverLifetime.maxY.min),
              0
            );
            break;
          case BezierCurve::Type::RandomBetweenCurves:
            particle.velocity = Vector3(
              Random::RandomFloatRange(
                ps.velocityOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.velocityOverLifetime.maxX.Evaluate(particle.lifeTimer / particle.lifeTime)),
              Random::RandomFloatRange(
                ps.velocityOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.velocityOverLifetime.maxY.Evaluate(particle.lifeTimer / particle.lifeTime)),
              0
            );
          }
          //switch (ps.rotationOverLifetime.type)
          //{
          //case BezierCurve::Type::Constant:
          //  particle.modifier.rotation *= Quaternion::AngleAxis(
          //    ps.rotationOverLifetime.min.min * static_cast<float>(DeltaTime()),
          //    Vector3::forward());
          //  break;
          //case BezierCurve::Type::ConstantCurve:
          //  particle.modifier.rotation *= Quaternion::AngleAxis(
          //    ps.rotationOverLifetime.min.Evaluate(particle.lifeTimer / particle.lifeTime) * static_cast<float>(DeltaTime()),
          //    Vector3::forward());
          //  break;
          //case BezierCurve::Type::RandomBetweenConstants:
          //  particle.modifier.rotation *= Quaternion::AngleAxis(
          //    Random::RandomFloatRange(
          //      ps.rotationOverLifetime.min.min,
          //      ps.rotationOverLifetime.max.min) * static_cast<float>(DeltaTime()),
          //    Vector3::forward());
          //  break;
          //case BezierCurve::Type::RandomBetweenCurves:
          //  particle.modifier.rotation *= Quaternion::AngleAxis(
          //    Random::RandomFloatRange(
          //      ps.rotationOverLifetime.min.Evaluate(particle.lifeTimer / particle.lifeTime),
          //      ps.rotationOverLifetime.max.Evaluate(particle.lifeTimer / particle.lifeTime)) * static_cast<float>(DeltaTime()),
          //    Vector3::forward());
          //}
          switch (ps.sizeOverLifetime.type)
          {
          case BezierCurve::Type::Constant:
            particle.modifier.scale = Vector3(
              ps.sizeOverLifetime.minX.min,
              ps.sizeOverLifetime.minY.min,
              1
            );
            break;
          case BezierCurve::Type::ConstantCurve:
            particle.modifier.scale = Vector3(
              ps.sizeOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
              ps.sizeOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
              1
              );
            break;
          case BezierCurve::Type::RandomBetweenConstants:
            particle.modifier.scale = Vector3(
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minX.min,
                ps.sizeOverLifetime.maxX.min),
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minY.min,
                ps.sizeOverLifetime.maxY.min),
              1
            );
            break;
          case BezierCurve::Type::RandomBetweenCurves:
            particle.modifier.scale = Vector3(
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minX.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.sizeOverLifetime.maxX.Evaluate(particle.lifeTimer / particle.lifeTime)),
              Random::RandomFloatRange(
                ps.sizeOverLifetime.minY.Evaluate(particle.lifeTimer / particle.lifeTime),
                ps.sizeOverLifetime.maxY.Evaluate(particle.lifeTimer / particle.lifeTime)),
              1
            );
          }

          Color min = ps.colorOverLifetime.min.Evaluate(particle.lifeTimer / particle.lifeTime);

          switch (ps.colorOverLifetime.type)
          {
          case Gradient::Type::ConstantColor:
          case Gradient::Type::ConstantGradient:
            particle.multiplier = min;
            break;
          case Gradient::Type::RandomBetweenColors:
          case Gradient::Type::RandomBetweenGradients:
            Color max = ps.colorOverLifetime.max.Evaluate(particle.lifeTimer / particle.lifeTime);
            particle.multiplier = Color(
              Random::RandomFloatRange(min.r, max.r),
              Random::RandomFloatRange(min.g, max.g),
              Random::RandomFloatRange(min.b, max.b),
              Random::RandomFloatRange(min.a, max.a)
            );
            break;
          }

          particle.transform.position += particle.velocity * static_cast<float>(DeltaTime());

          particle.lifeTimer += static_cast<float>(DeltaTime());
          ++index;
        }
      });
  }

  void ParticleSystem::LateUpdate()
  {

  }
}
