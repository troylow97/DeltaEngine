#include "PhysicsSystem.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/EngineClock.h"
#include "Collision.h"
#include <cmath>

namespace DeltaEngine
{
    void PhysicsSystem::Initialize()
    {
        m_gravity_amount = { 0,-60.0f };
        CurrentJumpTicks = 0;
        MaxJumpTicks = 10;
        CurrentDashTicks = 0;
        MaxDashTicks = 8;
        InitialJumpForce = 4500.0f;
        JumpForce = InitialJumpForce;
    }

    void PhysicsSystem::SetBounds(RigidBody& r1)
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
        else if (r1.FrictionCoeff > 5)
        {
            r1.FrictionCoeff = 5.0f;
        }
    }

    void PhysicsSystem::Update()
    {
      for ( size_t step = 0; step < env.pClock->Timesteps(); ++step )
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
                SetBounds(r1);

                //Set Euler
                t1.position += r1.Velocity * env.pClock->FixedDeltaTime();                

            	//Dash and Jump
            	if(em.HasComponent<Player>(id1))
            	{
                    Dash(em.GetComponent<Player>(id1),r1,c1);
                    Jump(em.GetComponent<Player>(id1), r1, c1);		
            	}

                //Movement
                Vector2 move = (r1.Direction * r1.Movespeed) + (r1.Direction * r1.InherentAcceleration * r1.AccelerationPickup);

                r1.AccumulatedForce += move * r1.Mass * 0.5f;

                //Apply Gravity
                if (r1.hasGravity && !c1.isCollidingOnFloor)
                {
                	if(!em.HasComponent<Player>(id1))
						r1.Acceleration = m_gravity_amount;
                }
                else
                {
                    r1.Acceleration = { 0,0 };
                }

                //Apply Friction
                const float dragForceMagnitude = (r1.Velocity.Length() * r1.FrictionCoeff);
                const Vector2 dragForceVector = (dragForceMagnitude * -(Normalise(r1.Velocity))) * env.pClock->DeltaTime();
                r1.Velocity += dragForceVector;


                //Apply Acceleration
                const Vector2 newAcceleration = r1.AccumulatedForce * (1 / r1.Mass) + r1.Acceleration;
                r1.Velocity += newAcceleration * env.pClock->FixedDeltaTime();

                //Apply Soft Drag
                r1.Velocity *= 0.96f;

                r1.AccumulatedForce = { 0,0 };
                c1.isCollidingOnFloor = false;
            }
            else
                r1.Velocity = { 0,0 }; //If non-moveable
        });

    }

	void PhysicsSystem::Dash(Player& p, RigidBody& r, Collider& c)
    {
        //Jumping,Dashing
        if (c.isCollidingOnFloor)
        {
            if (p.IsJumping)
            {
                CurrentJumpTicks = 1;
                JumpForce = InitialJumpForce;
            }
            else if (p.IsDashing && CurrentDashTicks < MaxDashTicks)
            {
                CurrentDashTicks++;
                if (p.DashDirectionRight)
                {
                    r.AccumulatedForce += Vector2{ 5000 + r.Mass * 100, 0 };
                }
                else
                {
                    r.AccumulatedForce -= Vector2{ 5000 + r.Mass * 100, 0 };
                }
            }
        }
        else if (p.IsDashing)
        {
            CurrentDashTicks = 0;
            p.IsDashing = false;
        }

        if (CurrentDashTicks >= MaxDashTicks)
        {
            CurrentDashTicks = 0;
            p.IsDashing = false;
        }
    }

	void PhysicsSystem::Jump(Player& p, RigidBody& r, Collider& c)
    {
        if (CurrentJumpTicks >= 1 && p.IsJumping)
        {
            r.AccumulatedForce += Vector2{ 0, JumpForce + r.Mass * 100 };
            JumpForce *= 0.7f;

            if (CurrentJumpTicks < MaxJumpTicks)
                CurrentJumpTicks++;
            else
            {
                p.IsJumping = false;
                CurrentJumpTicks = 0;
                JumpForce = InitialJumpForce;
            }
        }

        //Apply Gravity for player
        if (r.hasGravity && !c.isCollidingOnFloor && !p.IsDashing)
            r.Acceleration = m_gravity_amount;
        else
            r.Acceleration = { 0,0 };
    }
}