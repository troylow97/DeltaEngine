#include "PhysicsDrawSystem.h"
#include "Components/Collider.h"
#include "Core/Debugging/Gizmos.h"
#include "Input/InputManager.h"

namespace DeltaEngine
{

void PhysicsDrawSystem::Update()
{
  if ( InputManager::Get()->GetShowLine() == true )

    em.ForEach( e_query, [&]( EntityID id, Collider &c, Transform &t, RigidBody &r )
  {
    switch ( c.type )
    {
      case ColliderType::BOX:
        if ( InputManager::Get()->GetShowLine() == true )
        {
          Gizmos::DrawLine( t.position, t.position + r.Velocity );
        }
        Gizmos::Draw2DWireBox( t );
        break;
      case ColliderType::CIRCLE:
      {
        if ( InputManager::Get()->GetShowLine() == true )
        {
          Gizmos::DrawLine( t.position, t.position + r.Velocity );
        }
        Transform temp = t;
        temp.scale *= 2;
        Gizmos::Draw2DCircle( temp );
        break;
      }
      case ColliderType::RAY:
        if ( InputManager::Get()->GetShowLine() == true )
        {
          Gizmos::DrawLine( t.position, t.position + r.Velocity );
        }
        Gizmos::Draw2DWireBox( t ); //TO CHANGE
        break;
      case ColliderType::LINE:
        if ( InputManager::Get()->GetShowLine() == true )
        {
          Gizmos::DrawLine( t.position, t.position + r.Velocity );
        }
        Gizmos::Draw2DWireBox( t ); //TO CHANGE
        break;
      case ColliderType::NONE:
        if ( InputManager::Get()->GetShowLine() == true )
        {
          Gizmos::DrawLine( t.position, t.position + r.Velocity );
        }
        break;
    }
  } );
}
void PhysicsDrawSystem::LateUpdate()
{

}
}
