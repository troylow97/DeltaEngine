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
      em.ForEach([&](Transform& t, Player& p)
      {
        c_t->position = t.position;
        c_t->position.y = 0.0f;
      });
  }
}
