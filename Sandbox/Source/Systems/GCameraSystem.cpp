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
      em.ForEach([&](Transform& t, Player& p, RigidBody& r, Attack& atk)
        {
          float dist = Vector2::Distance(c_t->position, t.position);
          Vector3 pos = t.position;
          float distX = Math::Abs(c_t->position.x - pos.x);
          float distY = Math::Abs(c_t->position.y - pos.y);
          float offsetX = 1.f;
          float offsetY = 1.f;
          //pos.y = Math::Clamp(pos.y + 2, 2.f, 4.25f);
          if (distX > offsetX)
          {
            c_t->position.x = Math::MoveTowards(c_t->position.x, pos.x, (distX - offsetX) * (distX - offsetX) * DeltaTimef());
          }
          if (distY > offsetY)
          {
            c_t->position.y = Math::MoveTowards(c_t->position.y, pos.y, (distY - offsetY) * (distY - offsetY) * DeltaTimef());
          }

          float zspeed = 0.5f;
          if ((r.Velocity.x < 0.1f && r.Velocity.y < 0.1f &&
            r.Velocity.x > -0.1f && r.Velocity.y > -0.1f) || atk.MeleeCooldownTimer > 0)
          {
            pos.z = 4.5f;
            if (atk.MeleeCooldownTimer > 0)
            {
              pos.z = 4.f;
              zspeed = 5.f;
            }
          }
          else
          {
            pos.z = 6.5f;
            zspeed = 1;
          }
          c_t->position.z = Math::MoveTowards(c_t->position.z, pos.z, Math::Abs(c_t->position.z - pos.z) * DeltaTimef() * zspeed);
        });
  }
}
