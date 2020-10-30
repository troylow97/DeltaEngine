#include "InputSystem.h"
#include "Components/Character.h"
#include "Input/InputManager.h"

namespace DeltaEngine
{

void InputSystem::Update()
{
  if ( InputManager::Get()->IsKeyPressed( DEVK_A ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      i1.previousKey = DEVK_A;
      r1.Direction = Vector2::left();
    } );
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, Animator &a )
    {
      a.SetFloat( "Speed", 1.0f );
    } );
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, SpriteRenderer &s )
    {
      s.m_FlipX = true;
    } );
  }


  if ( InputManager::Get()->IsKeyReleased( DEVK_A ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, Animator &a )
    {
      a.SetFloat( "Speed", 0.0f );
    } );
  }
  if ( InputManager::Get()->IsKeyPressed( DEVK_D ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      i1.previousKey = DEVK_D;
      r1.Direction = Vector2::right();
    } );
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, Animator &a )
    {
      a.SetFloat( "Speed", 1.0f );
    } );
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, SpriteRenderer &s )
    {
      s.m_FlipX = false;
    } );
  }


  if ( InputManager::Get()->IsKeyReleased( DEVK_D ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, Animator &a )
    {
      a.SetFloat( "Speed", 0.0f );
    } );
  }
  if ( InputManager::Get()->IsKeyPressed( DEVK_W ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      i1.previousKey = DEVK_W;
      r1.Direction = Vector2::up();
    } );
  }
  else
  {
      env.pECS->GetWorld().get_entity_manager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1)
          {
              if (i1.previousKey = DEVK_D && r1.Direction.y > 0)
              {
                  r1.Direction *= 0.90f;
              }
          });
  }

  if ( InputManager::Get()->IsKeyPressed( DEVK_S ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      i1.previousKey = DEVK_S;
      r1.Direction = Vector2::down();
    } );
  }


  if ( InputManager::Get()->IsKeyTriggered( DEVK_BACKSLASH ) ) // '\'
  {
    if ( InputManager::Get()->GetShowLine() == false )
    {
        //std::cout << "V is triggered and line is shown" << std::endl;
      InputManager::Get()->SetShowLine( true );
    }
    else if ( InputManager::Get()->GetShowLine() == true )
    {
        //std::cout << "V is triggered and line is NOT shown" << std::endl;
      InputManager::Get()->SetShowLine( false );
    }
  }

  if (InputManager::Get()->IsKeyReleased(DEVK_A) || InputManager::Get()->IsKeyReleased(DEVK_D)
      || InputManager::Get()->IsKeyReleased(DEVK_W) || InputManager::Get()->IsKeyReleased(DEVK_S))
  {
      env.pECS->GetWorld().get_entity_manager().ForEach([&](EntityID id1, RigidBody& r1, Input& i1)
          {
              r1.Direction = Vector2::zero();
          });
  }


  // gets the coordinates of the mouse, good for debugging
  if ( InputManager::Get()->OnMouseMove() )
  {
    auto curr = InputManager::Get()->CurrentPosition();
    auto prev = InputManager::Get()->PreviousPosition();
    //if ( curr != prev )
    //  DeltaEngine_CORE_TRACE( "Mouse Position: x({}) y({})", curr.point_x, curr.point_y );
  }
}

void InputSystem::LateUpdate()
{

}


}
