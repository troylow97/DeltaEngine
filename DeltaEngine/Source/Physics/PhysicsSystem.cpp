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
        //UpdateComponents();
        UpdateVelocity();
    }

    void PhysicsSystem::LateUpdate()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
            {
                Vector2 NewPos = t1.position;
                //if (!c1.isWall)
                {
                    if (r1.PointEnd != Vector2::zero())
                    {
                        NewPos = r1.PointEnd;
                        c1.center = r1.PointEnd;
                        r1.PointEnd = Vector2::zero();
                    }
                    else
                    {
                        c1.center = t1.position + r1.Velocity;
                        NewPos = t1.position + r1.Velocity;
                    }

                    
                   em.ForEach([&](EntityID id2, RigidBody& r2, Collider& c2)
                   {
                       c1.center = NewPos;
                       if (id1.index != id2.index && CollisionIntersection_RectRect(c1,r1.Velocity,c2,Vector2::zero()))
                       {
                           r1.Velocity = Vector2::zero();
                           r1.PointEnd = Vector2::zero();
                           return;
                       }
                   });

                    t1.position = NewPos;

                }
            });
    }

    void PhysicsSystem::UpdateComponents()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1, Transform& t1, Collider& c1)
        {
            c1.size = t1.scale;
            c1.center = t1.position;

        });
    }

    void PhysicsSystem::UpdateVelocity()
    {
        em.ForEach([&](EntityID id1, RigidBody& r1,Transform& t1,Collider& c1)
        {
            //if (!c1.isWall)
            {
                //Player movement
                Vector2 move;
                move = (r1.Direction * r1.Movespeed);
                r1.AccumulatedForce += move;

                //set Euler
                //t1.position += r1.Velocity * env.pClock->DeltaTime();

                //Apply gravity
                if (r1.hasGravity && r1.Direction.y != 1)
                {
                    r1.Acceleration = m_gravity_amount;
                }

                Vector2 newAcceleration = r1.AccumulatedForce * 1 / r1.Mass + r1.Acceleration;
                r1.Velocity += newAcceleration * env.pClock->DeltaTime();

                //Apply Friction
                r1.Velocity *= std::pow(0.05f, env.pClock->DeltaTime());

                r1.AccumulatedForce = Vector2::zero();
            }
        });

    }

}