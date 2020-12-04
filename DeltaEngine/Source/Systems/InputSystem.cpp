#include "InputSystem.h"

#include "PhysicsDrawSystem.h"
#include "Components/Character.h"
#include "Input/InputManager.h"
#include "Components/Attack.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/Keys.h"

namespace DeltaEngine
{
void InputSystem::Initialize()
{
  DeltaEngine_CORE_TRACE( "Initialize Input System" );
}

void InputSystem::Shutdown()
{
  DeltaEngine_CORE_TRACE( "Shutdown Input System" );
}

float idle_timer { 0.0f };
float attack_cooldown { 0.0f };

void InputSystem::Update()
{
  for ( size_t i = 0; i < GetEnv().pClock->Timesteps(); i++ )
  {
    idle_timer += static_cast<float>( FixedDeltaTime() );
    attack_cooldown += static_cast<float>( FixedDeltaTime() );
  }

  env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID &id, RigidBody &r1, Input &i, State &a )
  {
    a.SetFloat( "IsIdle", idle_timer );

    if ( attack_cooldown > 0.5f )
    {
      a.SetBool( "Punch", false );
    }

    a.SetBool( "Ranged", false );
  } );

    if (InputManager::Instance().IsKeyPressed(DEVK_LEFT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
      {
        i1.previousKey = DEVK_A;
        r1.Direction = Vector2::left();
        if (r1.InherentAcceleration < r1.MaxAcceleration)
          r1.InherentAcceleration++;
        a.SetBool("IsRunning", true);
        idle_timer = 0.0f;
        i.m_FlipX = true;
      });
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_LEFT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a)
      {
        a.SetBool("IsRunning", false);
      });
    }
    if (InputManager::Instance().IsKeyPressed(DEVK_RIGHT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a, Image& i)
      {
        i1.previousKey = DEVK_D;
        r1.Direction = Vector2::right();
        if (r1.InherentAcceleration < r1.MaxAcceleration)
          r1.InherentAcceleration++;
    
        a.SetBool("IsRunning", true);
        idle_timer = 0.0f;
        i.m_FlipX = false;
      });
    }
    else if (InputManager::Instance().IsKeyReleased(DEVK_RIGHT))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1, State& a)
      {
        a.SetBool("IsRunning", false);
      });
    }

  //FOR TESTING-------------------------------------------------------------------------------------------------------
  if ( InputManager::Instance().IsKeyPressed( DEVK_UP ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
    {
      r1.Direction = Vector2::up();
      if ( r1.InherentAcceleration < r1.MaxAcceleration )
        r1.InherentAcceleration++;
    } );
  }
  else if ( InputManager::Instance().IsKeyReleased( DEVK_UP ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
    {
      r1.Direction = Vector2::zero();
      if ( r1.InherentAcceleration < r1.MaxAcceleration )
        r1.InherentAcceleration++;
    } );
  }

  if ( InputManager::Instance().IsKeyPressed( DEVK_DOWN ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
    {
      r1.Direction = Vector2::down();
      if ( r1.InherentAcceleration < r1.MaxAcceleration )
        r1.InherentAcceleration++;
    } );
  }
  else if ( InputManager::Instance().IsKeyReleased( DEVK_DOWN ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image &i )
    {
      r1.Direction = Vector2::zero();
      if ( r1.InherentAcceleration < r1.MaxAcceleration )
        r1.InherentAcceleration++;
    } );
  }
  //END TESTING-------------------------------------------------------------------------------------------------------


    if (InputManager::Instance().IsKeyTriggered(DEVK_SPACE))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1,State& a,Collider& c1, Player& p1, Input& i1)
      {
        if (c1.isCollidingOnFloor)
        {
            a.SetFloat("PlayerVelocityY", 1);
            p1.IsJumping = true;
        }
        i1.previousKey = DEVK_SPACE;
        idle_timer = 0.0f;
      });
    }
    if (InputManager::Instance().IsKeyReleased(DEVK_SPACE))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1,Player& p1, RigidBody& r1, Input& i1)
      {
        i1.previousKey = DEVK_SPACE;
        p1.IsJumping = false;
      });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_X)) //DASH
    {
        env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1,Transform& t1, RigidBody r1, State& s, Image& a, Collider& c1, Player& p1, Input& i1)
        {
            if (c1.isCollidingOnFloor && p1.AllowDashing)
            {
                if (a.m_FlipX)
                    p1.DashDirectionRight = false;
                else
                    p1.DashDirectionRight = true;
                p1.IsDashing = true;
                p1.AllowDashing = false;
                const EntityID missile = em.CreateEntity<Collider, Lifespan, Transform, RigidBody, EntityType, Health>();
                em.GetComponent<Transform>(missile).position = t1.position;
                em.GetComponent<RigidBody>(missile).Mass = 5.0f;
                em.GetComponent<Transform>(missile).scale = { 0.4f, 0.4f, 0.0f };
                em.GetComponent<Lifespan>(missile).Timer = 0.35f;
                em.GetComponent<Collider>(missile).isTrigger = true;
                em.GetComponent<EntityType>(missile).type = EntityCategory::E_PLAYER_DASH;
                em.GetComponent<RigidBody>(missile).FrictionCoeff = 0.0f;
                em.GetComponent<Health>(missile).CurrentHealth = 1;
            }
            i1.previousKey = DEVK_X;
        });
    }

    if (InputManager::Instance().IsKeyTriggered(DEVK_Z))
    {
      env.pECS->GetWorld().GetEntityManager().ForEach([&](EntityID id1, Input& i1, Attack& a1, Image& im, State& a)
      {
        a1.RangeAttack = true;
        a.SetBool("Ranged", true);
        idle_timer = 0.0f;
        attack_cooldown = 0.0f;
      });
    }

  if ( InputManager::Instance().IsKeyTriggered( DEVK_C ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, Input &i1, Attack &a1, Image &im, State &a )
    {
      a1.MeleeAttack = true;
      a.SetBool( "Punch", true );
      idle_timer = 0.0f;
      attack_cooldown = 0.0f;
    } );
  }

  if ( InputManager::Instance().IsKeyReleased( DEVK_UP ) || InputManager::Instance().IsKeyReleased( DEVK_DOWN )
       || InputManager::Instance().IsKeyReleased( DEVK_LEFT ) || InputManager::Instance().IsKeyReleased( DEVK_RIGHT ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      r1.Direction = Vector2::zero();
      r1.InherentAcceleration = 0.0f;
    } );
  }


  if ( InputManager::Instance().IsKeyTriggered( DEVK_BACKSLASH ) ) // '\'
    PhysicsDrawSystem::gizmo = !PhysicsDrawSystem::gizmo;


  if ( InputManager::Instance().IsKeyPressed( DEVK_LCTRL ) &&
       InputManager::Instance().IsKeyReleased( DEVK_RETURN ) )
    Profiler::Instance().Print();

  Profiler::Instance().Record( "Input System" );
}


void InputSystem::LateUpdate()
{}
}
