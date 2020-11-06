#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
#include <cmath>

namespace DeltaEngine
{
    void PhysicsSystem::Update()
    {
        UpdateVelocity();
    }

    void PhysicsSystem::LateUpdate()
    {
        
    }

    void PhysicsSystem::UpdateVelocity()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1,Transform& t1,Collider& c1)
        {
            if (r1.isMoveable)
            {
                //Check for infinite-mass
                if (r1.Mass <= 0)
                {
                    r1.Mass = 1.0f;
                }

                //Set Euler
                t1.position += r1.Velocity * env.pClock->DeltaTime();                

                if (r1.Direction == Vector2{ 0,-1 } && c1.isCollidingOnFloor)
                {
                
                }
                else if (r1.Direction == Vector2{ 0, 2 })
                {
                    r1.AccumulatedForce += {0, Jump_Force};
                    r1.Direction = Vector2::zero();
                }
                else
                {
                    //Player Movement
                    Vector2 move = (r1.Direction * r1.Movespeed);
                    r1.AccumulatedForce += move;
                }



                //Apply Gravity
                if (r1.hasGravity && !c1.isCollidingOnFloor)
                {
                    r1.Acceleration = m_gravity_amount;
                }
                else
                {
                    r1.Acceleration = { 0,0 };
                }

                //Apply Friction
                float dragForceMagnitude = (r1.Velocity.Length() * r1.FrictionCoeff);
                Vector2 dragForceVector = (dragForceMagnitude * -(Normalise(r1.Velocity))) * env.pClock->DeltaTime();
                if (dragForceVector.Magnitude() > std::numeric_limits<float>::epsilon())
                    r1.Velocity += dragForceVector;


                //Apply Acceleration
                Vector2 newAcceleration = r1.AccumulatedForce * (1 / r1.Mass) + r1.Acceleration;
                r1.Velocity += newAcceleration * env.pClock->DeltaTime();

                //Apply Soft Drag
                r1.Velocity *= 0.97f;

                //Clamp to velocity max for numerical stability
                if (Vector2DotProduct(r1.Velocity, r1.Velocity) > m_max_velocity)
                {
                    Normalise(r1.Velocity);
                    r1.Velocity = r1.Velocity * m_max_velocity;
                }

                r1.AccumulatedForce = { 0,0 };
                c1.isCollidingOnFloor = false;
            }
        });

    }

}