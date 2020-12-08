/**********************************************************************************
* \file   AI_StateMachine.h
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
#pragma once
#include "ECS/ECSModule.h"
#include "Components/AI.h"
#include "AI_State.h"

namespace DeltaEngine
{
  DEFINE_SYSTEM(AISystem, AI)
  public:

    AISystem::AISystem() = default;
    AISystem::~AISystem() = default;
    void AISystem::Initialize() override;
    void AISystem::LoadLevel1();
    void AISystem::Shutdown() override;
    void AISystem::Update() override;
    void AISystem::LateUpdate() override;
    std::unordered_map<std::string, AIState*> StateList; //All possible states AI can be in
  private:


  END_DEFINE_SYSTEM(AISystem)
}
