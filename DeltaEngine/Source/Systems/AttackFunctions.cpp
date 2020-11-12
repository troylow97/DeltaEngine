#include "AttackFunctions.h"

class Collider;

namespace DeltaEngine
{
	void PlayerRangedAttack(EntityID& id)
	{
		Transform& t1 = env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(id);

	}
}