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

                if (!c1.isWall)
                {
                    Vector2 move;
                    move = (r1.Direction * r1.Movespeed);
                    r1.Velocity += move * env.pClock->DeltaTime();
                    Vector2 norm_vel = Normalise(r1.Velocity);
                    c1.center += r1.Velocity;
                    em.ForEach([&](EntityID id2, Collider& c2)
                        {
                                if (id1.index != id2.index && CollisionIntersection_RectRect_Static(c1.center, c1.size, c2.center, c2.size))
                                {
                                    if (c2.isWall)
                                    {
                                        Manifold m;
                                        AABBvsAABB_Manifold(c1, c2, m);
                                        if (norm_vel == c1.collided_spot)
                                        {
                                            r1.Velocity = Vector2::zero();
                                            return;
                                        }
                                    }
                                }
                    
                        });
                    if (norm_vel == c1.collided_spot && c1.collided_spot != Vector2::zero())
                    {
                        c1.collided_spot = Vector2::zero();
                        return;
                    }

                    t1.position += r1.Velocity;

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
            if (!c1.isWall)
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
        Vector2 GravityAmount{ 0,-1.0f };
        em.ForEach([&](EntityID id1, RigidBody& r1,Transform& t1)
        {
            if ((r1.hasGravity == true))
            {
                r1.Velocity += GravityAmount;
            }
;
        });
    }

}