/**********************************************************************************
* \file   GCameraSystem.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "GCameraSystem.h"


#include "UnitManager.h"
#include "Audio/AudioEngine.h"
#include "Components/Components.h"
#include "../GameState.h"

namespace DeltaEngine
{
void GCameraSystem::Update()
{
}

void GCameraSystem::LateUpdate()
{
  bool exist { false };
  Transform *c_t { nullptr };
  em.ForEach( [&]( Transform &t, Camera &c )
  {
    exist = true;
    c_t = &t;
  } );

  if ( exist )
    em.ForEach( [&]( Transform &t, Player &p, RigidBody &r, Attack &atk, Image &img )
  {
    AudioEngine::Set3DListenerAttributes( { t.position, {},{0,0,1},{0,1,0} } );

    Vector3 pos = t.position;
    if (current == GameState::LEVEL_1)
    {
      pos.x = Math::Clamp(pos.x, -100.f, 210.f);
    }
    if ( img.m_FlipX )
      pos.x -= 3;
    else
      pos.x += 3;
    float distX = Math::Abs( c_t->position.x - pos.x );
    float distY = Math::Abs( c_t->position.y - pos.y );
    float offsetX = 1.f;
    float offsetY = 1.f;
    //pos.y = Math::Clamp(pos.y + 2, 2.f, 4.25f);
    if ( distX > offsetX )
    {
      float speed = Math::Clamp( distX - offsetX, 0.f, 32.f );
      c_t->position.x = Math::MoveTowards( c_t->position.x, pos.x,
                                           speed * speed * DeltaTimef() );
    }
    if ( distY > offsetY )
    {
      float speed = Math::Clamp( distY - offsetY, 0.f, 32.f );
      c_t->position.y = Math::MoveTowards( c_t->position.y, pos.y,
                                           speed * speed * DeltaTimef() );
    }

    float zspeed = 0.75f;
    if ( ( r.Velocity.x < 0.1f && r.Velocity.y < 0.1f &&
         r.Velocity.x > -0.1f && r.Velocity.y > -0.1f ) || atk.MeleeCooldownTimer > 0 )
    {
      pos.z = 4.5f;
      if ( atk.MeleeCooldownTimer > 0 )
      {
        pos.z = 4.f;
        zspeed = 5.f;
      }
    }
    else
    {
      pos.z = 6.5f;
      zspeed = 1.5f;
    }
    c_t->position.z = Math::MoveTowards( c_t->position.z, pos.z,
                                         Math::Abs( c_t->position.z - pos.z ) * DeltaTimef() * zspeed );
  } );

  UnitManager::Reset();
}


}
