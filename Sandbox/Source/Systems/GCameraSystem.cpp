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
#include "Components/Components.h"

namespace DeltaEngine
{
  void GCameraSystem::Update()
  {
  }

  void GCameraSystem::LateUpdate()
  {
    bool exist{false};
    Transform* c_t{nullptr};
    em.ForEach([&](Transform& t, Camera& c)
    {
      exist = true;
      c_t = &t;
    });

    if (exist)
      em.ForEach([&](Transform& t, Player& p, RigidBody& r)
        {
          float dist = Vector2::Distance(c_t->position, t.position);
          Vector3 pos = t.position;
          pos.y = Math::Clamp(pos.y + 2, 2.f, 4.f);
          pos.z = c_t->position.z;
          c_t->position = Vector3::MoveTowards(c_t->position, pos, dist * dist * DeltaTimef());
          //c_t->position.y = 2.0f;

          float zspeed = 0.5f;
          if (r.Velocity.x < 0.1f && r.Velocity.y < 0.1f &&
            r.Velocity.x > -0.1f && r.Velocity.y > -0.1f)
            pos.z = 5;
          else
          {
            pos.z = 8;
            zspeed = 1;
          }
          c_t->position.z = Math::MoveTowards(c_t->position.z, pos.z, Math::Abs(c_t->position.z - pos.z) * DeltaTimef() * zspeed);
        });
  }
}
