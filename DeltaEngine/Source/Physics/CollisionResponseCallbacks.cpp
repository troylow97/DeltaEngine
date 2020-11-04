#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
#include <cmath>

namespace DeltaEngine
{
	void CollisionResponseMain(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{
		ColliderType type1 = c1.type;

		switch (type1)
		{
		case ColliderType::BOX:
			return CollisionResponse_Sub_Box(c1, r1, c2, r2, m);
		case ColliderType::CIRCLE:
			return CollisionResponse_Sub_Circle(c1, r1, c2, r2, m);
		default:
			return CollisionResponse_Sub_Box(c1, r1, c2, r2, m);
		}
	}

	void CollisionResponse_Sub_Box(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{
		ColliderType type2 = c2.type;

		switch (type2)
		{
		case ColliderType::BOX:
			return CollisionResponse(c1, r1, c2, r2, m);
		case ColliderType::CIRCLE:
			return CollisionResponse_BoxCircle(c1, r1, c2, r2, m);
		default:
			return CollisionResponse(c1, r1, c2, r2, m);
		}
	}

	void CollisionResponse_Sub_Circle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{
		ColliderType type2 = c2.type;

		switch (type2)
		{
		case ColliderType::BOX:
			return CollisionResponse_BoxCircle(c2, r2, c1, r1, m);
		case ColliderType::CIRCLE:
			return CollisionResponse_CircleCircle(c1, r1, c2, r2, m);
		default:
			return CollisionResponse(c1, r1, c2, r2, m);
		}
	}

	void CollisionResponse(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{		
		float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
		Vector2 impulse, reflectedVectorA, reflectedVectorB;
		//calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
		reflectedVectorA = m.normal / (r1.Mass + r2.Mass) * r2.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
		reflectedVectorB = -m.normal / (r1.Mass + r2.Mass) * r1.Mass * (60 * m.penetration) * env.pClock->DeltaTime();

		if (restitution > std::numeric_limits<float>::epsilon())
		{
			if (r1.isMoveable)
			{
				float knockback_amt = 1 / r1.Mass * 20;
				impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
				r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;

			}

			if (r2.isMoveable)
			{
				float knockback_amt = 1 / r2.Mass * 20;
				impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
				r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
			}
		}
		else
		{
			if (r1.isMoveable)
			{
				impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
				r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
			}

			if (r2.isMoveable)
			{
				impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
				r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
			}
		}	
		r1.PointEnd = c1.center + reflectedVectorA;
		r2.PointEnd = c2.center + reflectedVectorB;
		//if (reflectedVectorA.Magnitude() > std::numeric_limits<float>::epsilon())
		//	r1.PointEnd = c1.center + reflectedVectorA;
		//else
		//	r1.PointEnd = c1.center;
		//if (reflectedVectorB.Magnitude() > std::numeric_limits<float>::epsilon())
		//	r2.PointEnd = c2.center + reflectedVectorB;
		//else
		//	r2.PointEnd = c2.center;
	}

	void CollisionResponse_BoxCircle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{
		r1.PointEnd = c1.center;
		r2.PointEnd = c2.center;
		r1.Velocity += m.normal + (m.normal * m.penetration * 5);
		r2.Velocity -= (m.normal + (m.normal * m.penetration * 5));

		//float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
		//Vector2 impulse, reflectedVectorA, reflectedVectorB;
		////calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
		//reflectedVectorA = m.normal / (r1.Mass + r2.Mass) * r2.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
		//reflectedVectorB = -m.normal / (r1.Mass + r2.Mass) * r1.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
		//
		//
		//if (restitution > std::numeric_limits<float>::epsilon())
		//{
		//	if (r1.isMoveable)
		//	{
		//		float knockback_amt = 1 / r1.Mass * 20;
		//		impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
		//		r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
		//		r1.PointEnd = c1.center + reflectedVectorA;
		//	}
		//
		//	if (r2.isMoveable)
		//	{
		//		float knockback_amt = 1 / r2.Mass * 20;
		//		impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
		//		r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
		//		r2.PointEnd = c2.center + reflectedVectorB;
		//	}
		//}
		//else
		//{
		//	if (r1.isMoveable)
		//	{
		//		impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
		//		r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
		//		r1.PointEnd = c1.center + reflectedVectorA;
		//	}
		//
		//	if (r2.isMoveable)
		//	{
		//		impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
		//		r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
		//		r2.PointEnd = c2.center + reflectedVectorB;
		//	}
		//}

	}

	void CollisionResponse_CircleCircle(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{
		float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
		Vector2 impulse, reflectedVectorA, reflectedVectorB;
		//calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
		reflectedVectorA = m.normal / (r1.Mass + r2.Mass) * r2.Mass * (60 * m.penetration) * env.pClock->DeltaTime();
		reflectedVectorB = -m.normal / (r1.Mass + r2.Mass) * r1.Mass * (60 * m.penetration) * env.pClock->DeltaTime();


		if (restitution > std::numeric_limits<float>::epsilon())
		{
			if (r1.isMoveable)
			{
				float knockback_amt = 1 / r1.Mass * 20;
				impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
				r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
				r1.PointEnd = c1.center + reflectedVectorA;
			}

			if (r2.isMoveable)
			{
				float knockback_amt = 1 / r2.Mass * 20;
				impulse = ((m.normal * m.penetration) + (knockback_amt * m.normal * restitution) + 1 * m.normal);
				r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
				r2.PointEnd = c2.center + reflectedVectorB;
			}
		}
		else
		{
			if (r1.isMoveable)
			{
				impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
				r1.Velocity += (impulse / (r1.Mass + r2.Mass)) * r2.Mass;
				r1.PointEnd = c1.center + reflectedVectorA;
			}

			if (r2.isMoveable)
			{
				impulse = ((m.normal * m.penetration) + 0.20 * m.normal);
				r2.Velocity -= (impulse / (r1.Mass + r2.Mass)) * r1.Mass;
				r2.PointEnd = c2.center + reflectedVectorB;
			}
		}
	}

}