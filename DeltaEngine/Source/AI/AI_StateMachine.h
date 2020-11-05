#pragma once
#include "ECS/ECSModule.h"
#include "AI_State.h"

namespace DeltaEngine
{
	namespace AI
	{
		DEFINE_SYSTEM(AISystem, RigidBody)
		public:
			AISystem::AISystem() = default;
			AISystem::~AISystem() = default;

			virtual void AISystem::Update() override;
			virtual void AISystem::LateUpdate() override;

		private:

		END_DEFINE_SYSTEM(AISystem)
	}
}