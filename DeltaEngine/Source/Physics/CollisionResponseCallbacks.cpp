#include "CollisionResponseCallbacks.h"
#include "Core/Math/Math.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
namespace DeltaEngine
{
	void CollisionResponse(Collider& c1, RigidBody& r1, Collider& c2, RigidBody& r2, Manifold& m)
	{
		//float seperating_velocity = Vector2DotProduct(r1.Velocity - r2.Velocity, m.normal);
		//if (seperating_velocity > 0)
		//	return;
		float restitution = Math::MathMin(r1.Restitution, r2.Restitution);

		//get the direction of reflection using dot product
		float aA = Vector2DotProduct(r1.Velocity, m.normal);
		float aB = Vector2DotProduct(r2.Velocity, m.normal);

		//calculate reflection vector based on conservation of momentum and direction based on the normal and velocity
		Vector2 reflectedVectorA = r1.Velocity + m.normal * r2.Mass * (2 * m.penetration / (r1.Mass + r2.Mass)) * restitution;
		Vector2 reflectedVectorB = r2.Velocity - m.normal * r1.Mass * (2 * m.penetration / (r1.Mass + r2.Mass)) * restitution;

		//update the end points of where the two objects will end up
		r1.PointEnd = c1.center + reflectedVectorA;
		r2.PointEnd = c2.center + reflectedVectorB;
		
	}

	void ResolveContactVelocity(RigidBody& r1, RigidBody& r2,Manifold& m)
	{
		float restitution = Math::MathMin(r1.Restitution, r2.Restitution);
		float seperating_velocity = Vector2DotProduct(r1.Velocity - r2.Velocity,m.normal);
		if (seperating_velocity > 0)
			return;

		//r1.Velocity += (m.normal * m.penetration);
		//r2.Velocity -= (m.normal * m.penetration);
		float newSepVelocity = -seperating_velocity * restitution;

		const bool AccelerationBuildUp = true;
		//When an object is resting on the ground it is constantly falling
		//due to gravity. This acceleration need be removed or objects will
		//jitter on the ground.

		if (AccelerationBuildUp)
		{
			// Check the velocity build-up due to acceleration only
			Vector2 accCausedVelocity = r1.Acceleration - r2.Acceleration;
			float accCausedSepVelocity = Vector2DotProduct(accCausedVelocity, m.normal) * env.pClock->DeltaTime();

			// If we've got a closing velocity due to acceleration build-up,
			// remove it from the new separating velocity
			if (accCausedSepVelocity < 0)
			{
				newSepVelocity += restitution * accCausedSepVelocity;

				// Make sure we haven't removed more than was
				// there to remove.
				if (newSepVelocity < 0) newSepVelocity = 0;
			}
		}

		//What is the total change in velocity for the contact?
		float deltaVelocity = newSepVelocity - seperating_velocity;

		//The delta velocity is applied to each object proportional to inverse
		//mass. So the more massive an object is the less of the change
		//in velocity it will receive.
		float totalInverseMass = 1 / r1.Mass + 1 / r2.Mass;

		// Calculate the impulse to apply
		float impulse = deltaVelocity / totalInverseMass;

		// Find the amount of impulse per unit of inverse mass
		Vector2 impulsePerIMass = m.normal * impulse;

		// Apply impulses: they are applied in the direction of the contact,
		// and in proportion to inverse mass.
		//r1.Velocity = r1.Velocity + impulsePerIMass * 1 / r1.Mass;
		//// The other body goes in the opposite direction
		//r2.Velocity = r2.Velocity + impulsePerIMass * -1/r2.Mass;

	}

	void ResolvePenetration(RigidBody& r1,Transform& t1, RigidBody& r2, Transform& t2, Manifold& m)
	{
		// The movement of each object is based on their inverse mass, so
		// total that.
		float totalInverseMass = 1 / r1.Mass + 1 / r2.Mass;
		
		// Find the amount of penetration resolution per unit of inverse mass
		Vector2 movePerIMass = m.normal * (m.penetration / totalInverseMass);
		
		//If stack stability can be increased by not resolving all the penetrations
		//in one step
		movePerIMass *= 0.8f;
		
		// Calculate the the movement amounts
		m.Movement[0] = movePerIMass * 1/r1.Mass;
		m.Movement[1] = movePerIMass * -1/r2.Mass;
		
		// Apply the penetration resolution
		t1.position += m.Movement[0];
		t2.position += m.Movement[1];
	}

}