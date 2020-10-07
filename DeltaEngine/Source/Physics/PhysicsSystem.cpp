#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"

namespace DeltaEngine
{
    void PhysicsSystem::Init()
    {
        GravityAmount = { 0, -1 };
    }

    void PhysicsSystem::update()
    {
        //Gravity();
        UpdateVelocity();
    }

    void PhysicsSystem::late_update()
    {
        // Codes
    }

    void PhysicsSystem::UpdateComponents()
    {

    }

    void PhysicsSystem::UpdateVelocity()
    {
        em.for_each([&](EntityID id1, RigidBody& r1,Transform& t1)
            {
                t1.old_position = t1.position;
                r1.Velocity += r1.Acceleration * env.pClock->DeltaTime(); //Apply Acceleration
                t1.position += r1.Velocity * env.pClock->DeltaTime(); //Update Position
                r1.Velocity *= 0.99f; //Apply Friction
                //DeltaEngine_CORE_TRACE(("Position: " , t1.position.x , "__" , t1.position.y));
            });
        //em.for_each([&](EntityID id1, Transform& t1)
        //    {
        //        std::cout << "t1.x position: " << t1.position.x << "   t1.y position: " << t1.position.y << std::endl;
        //    });

    }

    void PhysicsSystem::Gravity()
    {
        em.for_each([&](EntityID id1, RigidBody& r1,Transform& t1)
        {
            if(r1.hasGravity)
                r1.Velocity += GravityAmount;
        });
    }

}