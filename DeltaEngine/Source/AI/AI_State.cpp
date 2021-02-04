#include "AI_State.h"
#include "AI_Transition.h"
namespace DeltaEngine
{
	bool AIState::CheckEdges(EntityID& monster)
	{
		for (auto& it : TransitionEdges)
		{
			if (it.second->TestEdge(monster))
				return true;
		}

		return false;
	}
	AIState::~AIState()
	{
		for (auto& it : TransitionEdges)
		{
			delete it.second;
		}
	}
}