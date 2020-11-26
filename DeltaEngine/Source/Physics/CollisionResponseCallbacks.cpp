#include "CollisionSystem.h"
#include "Core/Math/Math.h"
#include "Collision.h"
#include <cmath>

namespace DeltaEngine
{
  //void CollisionSystem::CollisionResponseMain(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  //{
  //	ColliderType type1 = c1.type;
  //
  //	switch (type1)
  //	{
  //	case ColliderType::BOX:
  //		return CollisionResponse_Sub_Box(c1, r1, c2, r2, m);
  //	case ColliderType::CIRCLE:
  //		return CollisionResponse_Sub_Circle(c1, r1, c2, r2, m);
  //	default:
  //		return CollisionResponse_Sub_Box(c1, r1, c2, r2, m);
  //	}
  //}

  //void CollisionSystem::CollisionResponse_Sub_Box(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  //{
  //	ColliderType type2 = c2.type;
  //
  //	switch (type2)
  //	{
  //	case ColliderType::BOX:
  //		return CollisionResponse(c1, r1, c2, r2, m);
  //	case ColliderType::CIRCLE:
  //		return CollisionResponse_BoxCircle(c1, r1, c2, r2, m);
  //	default:
  //		return CollisionResponse(c1, r1, c2, r2, m);
  //	}
  //}

  //void CollisionSystem::CollisionResponse_Sub_Circle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  //{
  //	ColliderType type2 = c2.type;
  //
  //	switch (type2)
  //	{
  //	case ColliderType::BOX:
  //		return CollisionResponse_BoxCircle(c2, r2, c1, r1, m);
  //	case ColliderType::CIRCLE:
  //		return CollisionResponse_CircleCircle(c1, r1, c2, r2, m);
  //	default:
  //		return CollisionResponse(c1, r1, c2, r2, m);
  //	}
  //}

  void CollisionSystem::CollisionResponse(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  {
    //Get the separating velocity by projecting along the contact normal
    Vector2 relativeVelocity = r1.Velocity - r2.Velocity;
    Vector2 relativeVelocity2 = r2.Velocity - r1.Velocity;
    float SeperatingVelocity = Vector2DotProduct(relativeVelocity, m.normal);
    float SeperatingVelocity2 = Vector2DotProduct(relativeVelocity2, -m.normal);
    if (SeperatingVelocity > 0 || SeperatingVelocity2 > 0)
    {
        r1.PointEnd = c1.center;
        r2.PointEnd = c2.center;
        //The objects are no longer moving towards each other
        return;
    }

    //Calculate velocity using conservation of momentum
    float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
    float j = Vector2DotProduct(-(1 + restitution) * (r1.Velocity - r2.Velocity), m.normal)/(1/r1.Mass + 1/r2.Mass);

    if (r1.isMoveable)
        r1.Velocity = r1.Velocity + (j / r1.Mass) * m.normal;
    if(r2.isMoveable)
        r2.Velocity = r2.Velocity - (j / r2.Mass) * m.normal;

    //Snap object
    Vector2 impulse = (m.normal * m.penetration);
    Vector2 reflectedVectorA = ((impulse) / (r1.Mass + r2.Mass)) * r2.Mass;
    Vector2 reflectedVectorB = ((-impulse) / (r1.Mass + r2.Mass)) * r1.Mass;
    r1.PointEnd = c1.center + reflectedVectorA;
    r2.PointEnd = c2.center + reflectedVectorB;
  }

  //void CollisionSystem::CollisionResponse_BoxCircle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  //{
  //	r1.PointEnd = c1.center;
  //	r2.PointEnd = c2.center;
  //	r1.Velocity += m.normal + (m.normal * m.penetration * 5);
  //	r2.Velocity -= (m.normal + (m.normal * m.penetration * 5));
  //
  //	//float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
  //	//Vector2 impulse, reflectedVectorA, reflectedVectorB;
  //	////calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
  //	//reflectedVectorA = m.normal / (r1.Mass + r2.Mass) * r2.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
  //	//reflectedVectorB = -m.normal / (r1.Mass + r2.Mass) * r1.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
  //	//
  //	//
  //	//if (restitution > std::numeric_limits<float>::epsilon())
  //	//{
  //	//	if (r1.isMoveable)
  //	//	{
  //	//		float knockback_amt = 1 / r1.Mass * 20;
  //	//		impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
  //	//		r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
  //	//		r1.PointEnd = c1.center + reflectedVectorA;
  //	//	}
  //	//
  //	//	if (r2.isMoveable)
  //	//	{
  //	//		float knockback_amt = 1 / r2.Mass * 20;
  //	//		impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
  //	//		r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
  //	//		r2.PointEnd = c2.center + reflectedVectorB;
  //	//	}
  //	//}
  //	//else
  //	//{
  //	//	if (r1.isMoveable)
  //	//	{
  //	//		impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
  //	//		r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
  //	//		r1.PointEnd = c1.center + reflectedVectorA;
  //	//	}
  //	//
  //	//	if (r2.isMoveable)
  //	//	{
  //	//		impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
  //	//		r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
  //	//		r2.PointEnd = c2.center + reflectedVectorB;
  //	//	}
  //	//}
  //
  //}

  //void CollisionSystem::CollisionResponse_CircleCircle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
  //{
  //	float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
  //	Vector2 impulse, reflectedVectorA, reflectedVectorB;
  //	//calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
  //	reflectedVectorA = m.normal / (r1.Mass + r2.Mass) * r2.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
  //	reflectedVectorB = -m.normal / (r1.Mass + r2.Mass) * r1.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
  //
  //
  //	if (restitution > std::numeric_limits<float>::epsilon())
  //	{
  //		if (r1.isMoveable)
  //		{
  //			impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
  //			r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
  //			r1.PointEnd = c1.center + reflectedVectorA;
  //		}
  //
  //		if (r2.isMoveable)
  //		{
  //			impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
  //			r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
  //			r2.PointEnd = c2.center + reflectedVectorB;
  //		}
  //	}
  //	else
  //	{
  //		if (r1.isMoveable)
  //		{
  //			impulse = ((m.normal * m.penetration) + 0.20f * m.normal);
  //			r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
  //			r1.PointEnd = c1.center + reflectedVectorA;
  //		}
  //
  //		if (r2.isMoveable)
  //		{
  //			impulse = ((m.normal * m.penetration) + 0.20f * m.normal);
  //			r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
  //			r2.PointEnd = c2.center + reflectedVectorB;
  //		}
  //	}
  //}
}
