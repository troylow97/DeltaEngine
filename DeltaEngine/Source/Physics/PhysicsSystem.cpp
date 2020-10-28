#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
#include <stdlib.h> 

namespace DeltaEngine
{

    void PhysicsSystem::Update()
    {
        UpdateComponents();
        Gravity();
        UpdateVelocity();
    }

    void PhysicsSystem::LateUpdate()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
            {

                if (r1.isMoveable)
                {
                    Vector2 move;
                    move = (r1.Direction * r1.Movespeed);
                    r1.Velocity += move * env.pClock->DeltaTime();
                    Vector2 norm_vel = Normalise(r1.Velocity);

                    //If object is colliding onto something do not add to its position
                    if (c1.collided_spot == Vector2{ -10,-10 })
                    {
                        c1.collided_spot = Vector2::zero();
                    }
                    else if (norm_vel != c1.collided_spot)
                    {
                        c1.collided_spot = Vector2::zero();
                        t1.position += r1.Velocity;
                    }
                    else if (norm_vel == Vector2::zero())
                    {
                        t1.position += r1.Velocity;
                    }

                }

            });
    }

    void PhysicsSystem::UpdateComponents()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
        {
            c1.size = t1.scale;
            c1.center = t1.position;

            //if (r1.Friction.Age < r1.Friction.Lifetime)
            //{
            //    //if (r1.Friction.isActive == true)
            //    //{
            //        r1.Friction.Age += env.pClock->DeltaTime();
            //        r1.Friction.Magnitude *= (r1.Friction.Lifetime - r1.Friction.Age) * env.pClock->DeltaTime();
            //        r1.Velocity += (r1.Friction.UnitDirection * r1.Friction.Magnitude);
            //    //}
            //}
            //else
            //{
            //    r1.Friction.Age = 0;
            //    r1.Friction.isActive = false;
            //}

        });
    }

    void PhysicsSystem::UpdateVelocity()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1,Transform& t1,Collider& c1)
        {
            if (r1.isMoveable)
            {
                r1.Acceleration = r1.Direction;
                r1.Acceleration *= r1.inherentAcceleration;
                r1.Velocity += r1.Acceleration;
                r1.Velocity *= 0.99f;
                r1.Acceleration *= env.pClock->DeltaTime();
                r1.Velocity += r1.Acceleration;
                r1.Velocity *= env.pClock->DeltaTime();


            }
        });

    }

    void PhysicsSystem::Gravity()
    {
        Vector2 GravityAmount{ 0,-10.0f };
        em.ForEach([&](EntityID id1, RigidBody& r1,Transform& t1)
        {
            if ((r1.hasGravity == true))
            {
                r1.Velocity += GravityAmount / r1.Mass;
            }
;
        });
    }

}