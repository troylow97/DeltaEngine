#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
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
                    t1.position += r1.Velocity * env.pClock->DeltaTime(); //Update Position
                    r1.Velocity *= 0.97f; //Apply Friction

                //DeltaEngine_CORE_TRACE(("Position: " , t1.position.x , "__" , t1.position.y));
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
                r1.Velocity += r1.Direction * r1.Movespeed;
                r1.Velocity +=  r1.Acceleration * env.pClock->DeltaTime(); //Apply Acceleration

                //DeltaEngine_CORE_TRACE(("Position: " , t1.position.x , "__" , t1.position.y));
            });
        //em.for_each([&](EntityID id1, Transform& t1)
        //    {
        //        std::cout << "t1.x position: " << t1.position.x << "   t1.y position: " << t1.position.y << std::endl;
        //    });

    }

    void PhysicsSystem::Gravity()
    {
        Vector2 GravityAmount{ 0,-0.2 };
        em.for_each([&](EntityID id1, RigidBody& r1,Transform& t1)
        {
            if(r1.hasGravity)
                r1.Velocity += GravityAmount;
        });
    }

}