#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"
#include "Collision.h"
#include <cmath>

namespace DeltaEngine
{
    void PhysicsSystem::Initialize()
    {
        m_gravity_amount = { 0,-60.0f };
        CurrentJumpTicks = 0;
        MaxJumpTicks = 10;
        InitialJumpForce = 5000.0f;
        JumpForce = InitialJumpForce;
        m_max_velocity = 1000.0f;
    }

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

                if (r1.FrictionCoeff <= 0)
                {
                    r1.FrictionCoeff = 0.01f;
                }

                //Set Euler
                t1.position += r1.Velocity * env.pClock->DeltaTime();                

                //Jumping
                if (r1.isJumping == true && c1.isCollidingOnFloor)
                {
                    CurrentJumpTicks = 1;
                    JumpForce = InitialJumpForce;
                }

                if (CurrentJumpTicks >= 1 && r1.isJumping == true)
                {
                    std::cout << "CurrentJumpTicks: " << CurrentJumpTicks << std::endl;
                    r1.AccumulatedForce += Vector2{ 0, JumpForce + r1.Mass * 100 };
                    JumpForce *= 0.8f;

                    if(CurrentJumpTicks < MaxJumpTicks)
                        CurrentJumpTicks++;
                    else
                    {
                        r1.isJumping = false;
                        CurrentJumpTicks = 0;
                        JumpForce = InitialJumpForce;
                    }
                }


                //
                if (r1.Direction == Vector2{ 0,-1 } && c1.isCollidingOnFloor)
                {
                
                }
                else
                {
                    //Player Movement
                    Vector2 move = (r1.Direction * r1.Movespeed) + (r1.Direction * r1.InherentAcceleration * r1.AccelerationPickup);

                    r1.AccumulatedForce += move * r1.Mass* 0.5f;
                }      

                //Apply Gravity
                if (r1.hasGravity)
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
                    r1.Velocity = r1.Velocity + m_max_velocity;
                }

                r1.AccumulatedForce = { 0,0 };
                c1.isCollidingOnFloor = false;
            }
            else
                r1.Velocity = { 0,0 };
        });

    }

}