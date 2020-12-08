/**********************************************************************************
* \file   PhysicsDrawSystem.cpp
* \brief  The file contains implementation of PhysicsDrawSystem.
* \author Ong, Graeme,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "PhysicsDrawSystem.h"
#include "Components/Collider.h"
#include "Core/Debugging/Gizmos.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/InputManager.h"
#include "Core/GlobalStruct.h"
#include "Systems/InputSystem.cpp"
#include "ImGui/Panels/WorldPanel.cpp"

namespace DeltaEngine
{
void PhysicsDrawSystem::Update()
{
  if ( Editor::entity_selected && Editor::tool_selection == Editor::Tool::EntitySelector )
  {
    Gizmos::SetColor( { 0.19f, 0.71f, 0.81f, .5f } );
    Gizmos::Draw2DBox( Editor::selection_transform );
    Gizmos::SetColor( { 0.19f, 0.71f, 0.81f, 1.f } );
    Gizmos::Draw2DWireBox( Editor::selection_transform );
    Gizmos::SetColor();
  }

  if ( gizmo )
  {
    em.ForEach( e_query, [&]( EntityID id, Collider &c, Transform &t, RigidBody &r )
    {
      Transform rTransform = t;
      Vector2 pos = c.center + c.offset;
      switch ( c.type )
      {
        case ColliderType::BOX:
          rTransform.scale *= c.size;
          Gizmos::DrawLine( pos, c.center + r.Velocity );
          Gizmos::Draw2DWireBox( rTransform, c.offset );
          break;
        case ColliderType::CIRCLE:
        {
          Gizmos::DrawLine( pos, c.center + r.Velocity );
          Transform temp = t;
          temp.scale *= 2;
          Gizmos::Draw2DCircle( temp );
          break;
        }
        case ColliderType::RAY:
          Gizmos::DrawLine( pos, c.center + r.Velocity );
          Gizmos::Draw2DWireBox( t ); //TO CHANGE
          break;
        case ColliderType::LINE:
          Gizmos::DrawLine( pos, c.center + r.Velocity );
          Gizmos::Draw2DWireBox( t ); //TO CHANGE
          break;
        case ColliderType::NONE:
          Gizmos::DrawLine( c.center, c.center + r.Velocity );
          break;
      }
    } );
  }
  Profiler::Instance().Record( "Physics Draw System" );
}

void PhysicsDrawSystem::LateUpdate()
{}
}
