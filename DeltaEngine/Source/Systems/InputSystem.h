/**********************************************************************************
* \file   InputSystem.h
* \brief  The file contains interface of InputSystem.
* \author Chin, Clara,   50% Code Contribution
* \author Low, Troy,     50% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(InputSystem, Input)
  public:
    static bool GetGodMode();
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    void SetIdleAnimation();
    void GodMode();
    void RunLeft();
    void RunRight();
    void StopRun();
    void Jump();
    void Dash();
    void Dodge();
    void Punch();
    void Shield();
    void Shoot();
    // void Upgrade();
    void Shutdown() override;
  private:
    static bool god_mode;
  END_DEFINE_SYSTEM(InputSystem)
}
