#pragma once
#include "ECS/ECSModule.h"
//#include "AI_State.h"
#include "Components/AI.h"
#include "AI_State.h"
#include <unordered_map>
namespace DeltaEngine
{
	DEFINE_SYSTEM(AISystem, AI)
	public:
		AISystem::AISystem() = default;
		AISystem::~AISystem() = default;
		void AISystem::Init();
		virtual void AISystem::Update() override;
		virtual void AISystem::LateUpdate() override;
		std::unordered_map<std::string, AIState*> StateList; //All possible states AI can be in
	private:


	END_DEFINE_SYSTEM(AISystem)

}