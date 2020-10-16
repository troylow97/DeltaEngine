#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
#include <stdlib.h> 

namespace DeltaEngine
{

    void PhysicsSystem::update()
    {
        UpdateComponents();
        Gravity();
        UpdateVelocity();
    }

    void PhysicsSystem::late_update()
    {
        // Codes
        em.for_each([&](EntityID id1, RigidBody& r1, Transform& t1,Collider& c1)
            {
                if (r1.isMoveable)
                {



                }
            });

    }

    void PhysicsSystem::UpdateComponents()
    {
        em.for_each([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
        {
            c1.size = t1.scale;
            c1.center = t1.position;
        });
    }

    void PhysicsSystem::UpdateVelocity()
    {
        em.for_each([&](EntityID id1, RigidBody& r1,Transform& t1)
            {
                if (r1.isMoveable)
                {
                    r1.Movespeed = 105.0f;
                    r1.inherentAcceleration = 3.0f;



                    r1.Direction *= r1.Movespeed;
                    r1.Acceleration *= r1.Direction;
                    r1.Velocity += r1.Acceleration;
                    r1.Velocity *= r1.Friction; 

                    r1.Acceleration *= env.pClock->DeltaTime();
                    r1.Velocity += r1.Acceleration;
                    r1.Velocity *= env.pClock->DeltaTime();
                   // r1.Velocity += ((r1.Direction * (r1.Movespeed)) / r1.Mass); //Update Velocity
                    t1.position += r1.Velocity; //Update Position
                    std::cout << "Velocity: " << r1.Velocity.x << "," << r1.Velocity.y << std::endl;
                }
            });

    }

    void PhysicsSystem::Gravity()
    {
        Vector2 GravityAmount{ 0,-5.0f };
        em.for_each([&](EntityID id1, RigidBody& r1,Transform& t1)
        {
            if ((r1.hasGravity == true))
            {
                r1.Velocity += GravityAmount / r1.Mass;
            }
;
        });
    }

}