#pragma once

#include "Core/Math/DE_Math.h"
#include "Core/Math/BezierCurve.h"
#include "Core/Math/Gradient.h"

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
    float startLifetimeMin = 1, startLifetimeMax = 1;
    Vector3 startVelocityMin, startVelocityMax;
    Color startColorMin, startColorMax;
    float startRotationMin, startRotationMax;
    Vector3 startSizeMin = Vector3::one(), startSizeMax = Vector3::one();

    //over time props
    BezierCurve velocityOverLifetimeXMin, velocityOverLifetimeYMin, velocityOverLifetimeZMin;
    BezierCurve velocityOverLifetimeXMax, velocityOverLifetimeYMax, velocityOverLifetimeZMax;
    Gradient colorOverLifetimeMin;
    Gradient colorOverLifetimeMax;
    float rotationOverLifetimeMin;
    float rotationOverLifetimeMax;
    BezierCurve sizeOverLifetimeXMin, sizeOverLifetimeYMin, sizeOverLifetimeZMin;
    BezierCurve sizeOverLifetimeXMax, sizeOverLifetimeYMax, sizeOverLifetimeZMax;

    bool playOnAwake = true;
    unsigned int maxParticles = 100;

    //emission
    int rateOverTime = 30;
    std::vector<Burst> bursts{/* {1, 3} */};

    //shape
    Shape shape = Shape::Circle;
    GenType genType = GenType::Spread;
    float radius = 1.f;
    Transform shapeTransform = Transform();

    // these should not be easily editable
    // they are public for system access

    struct Particle
    {
      float lifeTime = 1.0f;
      float lifeTimer = 0.0f;

      Transform transform;

      Vector3 velocity;
      Color color;

      bool active = false;
    };

    std::vector<Particle> m_ParticlePool;
    unsigned int m_ActiveParticles = 0;
    float emissionTimer = 0;
    float durationTimer = 0;
    float playbackTimer = 0;
  };
}
