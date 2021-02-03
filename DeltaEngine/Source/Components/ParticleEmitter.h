#pragma once

#include "Core/Math/DE_Math.h"

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

    //properties
    float duration = 5;
    bool looping = true;
    bool prewarm = false;
    float startDelay = 0;
    float startLifetime[2] = { 1.0f, 1.0f };
    Vector3 startVelocity[2] = { Vector3(), Vector3() };
    Color startColor[2] = { Color(), Color() };
    float startRotation[2] = { 0, 0 };
    float startSize[2] = { 1, 1 };

    bool playOnAwake = true;
    unsigned int maxParticles = 100;

    //emission
    int rateOverTime = 30;
    std::vector<Burst> bursts;

    //shape
    Shape shape;
    float radius;
    Transform shapeTransform;

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
    float durationTimer = 0;
  };

}
