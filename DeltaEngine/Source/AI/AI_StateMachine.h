#pragma once
#include "ECS/ECSModule.h"
#include "Components/AI.h"

class AIState;

namespace DeltaEngine
{
	DEFINE_SYSTEM(AISystem, AI)
	public:

		AISystem::AISystem() = default;
		AISystem::~AISystem() = default;
		void AISystem::Initialize();
		void AISystem::Shutdown();
		virtual void AISystem::Update() override;
		virtual void AISystem::LateUpdate() override;
		std::unordered_map<std::string, AIState*> StateList; //All possible states AI can be in
	private:


	END_DEFINE_SYSTEM(AISystem)

}