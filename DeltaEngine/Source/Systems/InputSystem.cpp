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
      r1.Velocity = { -1, 0 };
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
  else if ( InputManager::Get()->IsKeyReleased( DEVK_A ) )
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
      r1.Velocity = { 1, 0 };
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
  else if ( InputManager::Get()->IsKeyReleased( DEVK_D ) )
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
      r1.Velocity = { 0, 1 };
    } );
  }
  if ( InputManager::Get()->IsKeyPressed( DEVK_S ) )
  {
    env.pECS->GetWorld().get_entity_manager().ForEach( [&]( EntityID id1, RigidBody &r1, Input &i1 )
    {
      i1.previousKey = DEVK_S;
      r1.Velocity = { 0, -1 };
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

  if (InputManager::Get()->IsMouseTriggered(DEVK_LBUTTON))
  {
    env.pECS->GetWorld().get_entity_manager().ForEach([&](EntityID& id1, Collider& c1, Transform& t1, RigidBody& r1)
	{
	  if (c1.type == ColliderType::BOX)
	  {
	    if (CollisionIntersection_RectMouse(c1.center, c1.size, InputManager::Get()->CurrentCameraPosition()))
	    {
		  InputManager::Get()->SetEntitySelected(true);
		  InputManager::Get()->SetEntityIDSelected(id1.index);
	    }
	  }
	  else if (c1.type == ColliderType::CIRCLE)
	  {
        if (CollisionIntersection_RectMouse(c1.center, c1.size, InputManager::Get()->CurrentCameraPosition()))
        {
		  InputManager::Get()->SetEntitySelected(true);
		  InputManager::Get()->SetEntityIDSelected(id1.index);
		  //std::cout << "entity selected is " << InputManager::Get()->EntityIDSelected() << std::endl;
        }
	  }
	});
  }

  //if ( InputManager::Get()->IsKeyTriggered( DEVK_P ) )
  //{
  //  em.ForEach( [&]( EntityID &id, Input &input )
  //  {
  //    auto t2 = em.GetComponent<Transform>( id );
  //    auto circleEntity = env.pECS->GetWorld().get_entity_manager().CreateEntity<Transform, RigidBody, Collider>();
  //    auto &circleT = env.pECS->GetWorld().get_entity_manager().GetComponent<Transform>( circleEntity );
  //    auto &circleR = env.pECS->GetWorld().get_entity_manager().GetComponent<RigidBody>( circleEntity );
  //    auto &circleC = env.pECS->GetWorld().get_entity_manager().GetComponent<Collider>( circleEntity );

  //    circleT.position = Vector3( t2.position.x, t2.position.y - 0.25f );
  //    circleT.scale = Vector3( 0.1, 0.1 );
  //    circleR.hasGravity = true;
  //    circleC.type = ColliderType::CIRCLE;
  //  } );
  //}


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
