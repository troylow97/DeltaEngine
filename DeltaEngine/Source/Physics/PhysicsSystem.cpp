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
        //Apply Friction


        //em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
        //    {
        //        Vector2 NewPos = t1.position;
        //        if (!c1.isWall)
        //        {
        //            if (r1.PointEnd != Vector2::zero())
        //            {
        //                NewPos = r1.PointEnd;
        //                r1.PointEnd = Vector2::zero();
        //            }
        //            else
        //            {
        //                NewPos = t1.position + r1.Velocity;
        //            }
        //
        //            t1.position = NewPos;
        //
        //        }
        //    });
    }

    void PhysicsSystem::UpdateVelocity()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1,Transform& t1,Collider& c1)
        {
            if (r1.isMoveable)
            {
                //if (r1.Direction == Vector2{ 0,-1 } && c1.isCollidingOnFloor)
                //{
                //    return;
                //}

                //Set Euler
                t1.position += r1.Velocity * env.pClock->DeltaTime();                

                //Player movement
                Vector2 move = (r1.Direction * r1.Movespeed);
                r1.AccumulatedForce += move;


                //Apply gravity
                if (r1.hasGravity && !c1.isCollidingOnFloor)
                {
                    r1.Acceleration = m_gravity_amount;
                }

                Vector2 newAcceleration = r1.AccumulatedForce * (1 / r1.Mass) + r1.Acceleration;
                r1.Velocity += newAcceleration * env.pClock->DeltaTime();

                //Apply Soft Drag
                r1.Velocity -= r1.Velocity * r1.FrictionCoeff;
                r1.AccumulatedForce = Vector2::zero();
            }
        });

    }

}