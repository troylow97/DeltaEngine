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
                r1.Velocity *= r1.Friction; //Apply Friction
                t1.position += r1.Velocity * env.pClock->DeltaTime(); //Update Position
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
                r1.Velocity += ((r1.Direction * (r1.Movespeed)) / r1.Mass); //Update Velocity
                r1.Velocity +=  r1.Acceleration * env.pClock->DeltaTime();  //Apply Acceleration
                r1.Acceleration = (r1.Direction * r1.inherentAcceleration); //Increase Acceleration
            });

    }

    void PhysicsSystem::Gravity()
    {
        Vector2 GravityAmount{ 0,-5.0f };
        em.for_each([&](EntityID id1, RigidBody& r1,Transform& t1)
        {
            if (r1.hasGravity)
            {
                r1.Velocity += GravityAmount * 1 / r1.Mass;
                r1.Acceleration = (GravityAmount * r1.inherentAcceleration) / r1.Mass;
            }
;
        });
    }

}