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
