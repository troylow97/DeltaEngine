/**********************************************************************************
* \file   CollisionsResponseCallbacks.cpp
* \brief  The file contains the logic for collision response
* \author Low, Troy,     100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "CollisionSystem.h"
#include "Core/Math/Math.h"
#include "Collision.h"
#include <cmath>

namespace DeltaEngine
{
  void CollisionSystem::CollisionResponse(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  {
    //Get the separating velocity by projecting along the contact normal
    const Vector2 relativeVelocity = r1.Velocity - r2.Velocity;
    const Vector2 relativeVelocity2 = r2.Velocity - r1.Velocity;
    const float SeparatingVelocity = Vector2DotProduct(relativeVelocity, m.normal);
    const float SeparatingVelocity2 = Vector2DotProduct(relativeVelocity2, -m.normal);
    if (SeparatingVelocity > 0 || SeparatingVelocity2 > 0)
    {
      r1.PointEnd = c1.center;
      r2.PointEnd = c2.center;
      //The objects are no longer moving towards each other
      return;
    }

    //Calculate velocity using conservation of momentum
    float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
    float j = Vector2DotProduct(-(1 + restitution) * (r1.Velocity - r2.Velocity), m.normal) / (1 / r1.Mass + 1 / r2.Mass );

    if (r1.isMoveable)
      r1.Velocity = r1.Velocity + (j / r1.Mass) * m.normal;
    if (r2.isMoveable)
      r2.Velocity = r2.Velocity - (j / r2.Mass) * m.normal;

    //Snap object
    const Vector2 impulse = (m.normal * m.penetration);
    const Vector2 reflectedVectorA = ((impulse) / (r1.Mass + r2.Mass)) * r2.Mass;
    const Vector2 reflectedVectorB = ((-impulse) / (r1.Mass + r2.Mass)) * r1.Mass;
    r1.PointEnd = c1.center + reflectedVectorA;
    r2.PointEnd = c2.center + reflectedVectorB;
  }
}
