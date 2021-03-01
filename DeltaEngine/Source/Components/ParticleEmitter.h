#pragma once

#include "Core/Math/DE_Math.h"
#include "Core/Math/BezierCurve.h"
#include "Core/Math/Gradient.h"
#include "Components/Transform.h"
#include "ECS/Entities.h"

namespace DeltaEngine
{
  struct ParticleEmitter
  {
    struct Burst
    {
      float time;
      int count;
    };

    enum class Shape
    {
      None,
      Circle,
      Line,
      Box
    };

    enum class GenType
    {
      Random,
      Loop,
      PingPong,
      Spread
    };

    //properties
    float duration = 10;
    bool looping = true;
    bool prewarm = false;

    //instantiate props
    float startDelay = 0;
    float startLifetimeMin = 5, startLifetimeMax = 5;
    Vector3 startVelocityMin, startVelocityMax;
    Color startColorMin, startColorMax;
    float startRotationMin = 0, startRotationMax = 0;
    Vector3 startSizeMin = Vector3::one(), startSizeMax = Vector3::one();

    //over time props
    BezierRange3 velocityOverLifetime;
    GradientRange colorOverLifetime;
    BezierRange rotationOverLifetime;
    BezierRange3 sizeOverLifetime;

    bool playOnAwake = true;
    unsigned maxParticles = 1;

    //emission
    int rateOverTime = 1;
    //std::vector<Burst> bursts{/* {1, 3} */};

    //shape
    Shape shape = Shape::Circle;
    GenType genType = GenType::Spread;
    float radius = 1.f;
    Transform shapeTransform = Transform();

    // these should not be easily editable
    // they are public for system access

    struct Particle
    {
      float lifeTime = 5.0f;
      float lifeTimer = 0.0f;

      Transform transform;

      Vector3 velocity;
      Color color;

      bool active = false;
    };

    unsigned m_ActiveParticles = 0;
    float emissionTimer = 0;
    float durationTimer = 0;
    
    static std::map<EntityID, std::vector<Particle>> particlePools;
  };
}
