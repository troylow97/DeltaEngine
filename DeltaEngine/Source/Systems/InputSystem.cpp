#include "InputSystem.h"
#include "Components/Character.h"
#include "Input/InputManager.h"
#include "Components/Attack.h"
#include "Core/GlobalStruct.h"
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
    idle_timer += FixedDeltaTime();
    attack_cooldown += FixedDeltaTime();
  }

  env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID &id,  RigidBody &r1,Input &i, State &a )
  {
    a.SetFloat( "IsIdle", idle_timer );
    //if (abs(r1.Velocity.y) > 1.0f )
    //a.SetFloat( "PlayerVelocityY", r1.Velocity.y  );
    //if (!r1.isJumping &&  abs(r1.Velocity.y) > 1.0f)
    //  a.SetFloat( "PlayerVelocityY", -r1.Velocity.y );
    if (attack_cooldown > 0.5f)
    {
      a.SetBool( "Punch", false );
    }
    
    a.SetBool( "Ranged", false );

  } );

  if ( InputManager::Get()->IsKeyPressed( DEVK_LEFT ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image&i )
    {
      i1.previousKey = DEVK_A;
      r1.Direction = Vector2::left();
      if ( r1.InherentAcceleration < r1.MaxAcceleration )
        r1.InherentAcceleration++;
      a.SetBool( "IsRunning", true );
      idle_timer = 0.0f;
      i.m_FlipX = true;
    } );
  }
  else if ( InputManager::Get()->IsKeyReleased( DEVK_LEFT ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a )
    {
      a.SetBool( "IsRunning", false );
    } );
  }
  if ( InputManager::Get()->IsKeyPressed( DEVK_RIGHT ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a, Image&i )
    {
      i1.previousKey = DEVK_D;
      r1.Direction = Vector2::right();
      if ( r1.InherentAcceleration < r1.MaxAcceleration )
        r1.InherentAcceleration++;

      a.SetBool( "IsRunning", true );
      idle_timer = 0.0f;
      i.m_FlipX = false;

    } );

  }
  else if ( InputManager::Get()->IsKeyReleased( DEVK_RIGHT ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1, State &a )
    {
      a.SetBool( "IsRunning", false );
    } );
  }

  if ( InputManager::Get()->IsKeyTriggered( DEVK_SPACE ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Collider &c1, Input &i1 )
    {
      if ( c1.isCollidingOnFloor )
      {
        r1.isJumping = true;
      }
      i1.previousKey = DEVK_SPACE;
      idle_timer = 0.0f;
    } );
  }

  if ( InputManager::Get()->IsKeyReleased( DEVK_SPACE ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      i1.previousKey = DEVK_SPACE;
      r1.isJumping = false;
    } );
  }

  if ( InputManager::Get()->IsKeyTriggered( DEVK_C ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, Input &i1, Attack &a1, Image &im, State& a )
    {
      a1.RangeAttack = true;
      a.SetBool( "Ranged", true );
      idle_timer = 0.0f;

    } );
  }

  if ( InputManager::Get()->IsKeyTriggered( DEVK_X ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, Input &i1, Attack &a1, Image &im, State& a )
    {
      a1.MeleeAttack = true;
      a.SetBool( "Punch", true );
      idle_timer = 0.0f;
      attack_cooldown = 0.0f;
    } );
  }

  if ( InputManager::Get()->IsKeyReleased( DEVK_UP ) || InputManager::Get()->IsKeyReleased( DEVK_DOWN )
       || InputManager::Get()->IsKeyReleased( DEVK_LEFT ) || InputManager::Get()->IsKeyReleased( DEVK_RIGHT ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      r1.Direction = Vector2::zero();
      r1.InherentAcceleration = 0.0f;
    } );
  }


  if ( InputManager::Get()->IsKeyTriggered( DEVK_BACKSLASH ) ) // '\'
    if ( InputManager::Get()->GetShowLine() == false )
      InputManager::Get()->SetShowLine( true );
    else if ( InputManager::Get()->GetShowLine() == true )
      InputManager::Get()->SetShowLine( false );

  // gets the coordinates of the mouse, good for debugging
  if ( InputManager::Get()->OnMouseMove() )
  {
    auto curr = InputManager::Get()->CurrentPosition();
    auto prev = InputManager::Get()->PreviousPosition();
    //if ( curr != prev )
    //  DeltaEngine_CORE_TRACE( "Mouse Position: x({}) y({})", curr.point_x, curr.point_y );
  }

  if ( InputManager::Get()->IsMouseTriggered( DEVK_LBUTTON ) )
  {
    env.pECS->GetWorld().GetEntityManager().ForEach( [&]( EntityID &id1, Collider &c1, Transform &t1, RigidBody &r1 )
    {
      if ( c1.type == ColliderType::BOX )
      {
        if ( CollisionIntersection_RectMouse( c1.center, c1.size, InputManager::Get()->CurrentCameraPosition() ) )
        {
          InputManager::Get()->SetEntitySelected( true );
          InputManager::Get()->SetEntityIDSelected( id1.index );
        }
      }
      else if ( c1.type == ColliderType::CIRCLE )
      {
        if ( CollisionIntersection_RectMouse( c1.center, c1.size, InputManager::Get()->CurrentCameraPosition() ) )
        {
          InputManager::Get()->SetEntitySelected( true );
          InputManager::Get()->SetEntityIDSelected( id1.index );
          //std::cout << "entity selected is " << InputManager::Get()->EntityIDSelected() << std::endl;
        }
      }
    } );
  }

}

void InputSystem::LateUpdate()
{

}


}
