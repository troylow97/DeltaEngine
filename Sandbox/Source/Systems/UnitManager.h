#pragma once
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(UnitManager, Player)
    void Initialize() override;
    void Update() override;
    void LateUpdate() override;
    static EntityID GetPlayerID();
private:
    static EntityID playerID;
	
    END_DEFINE_SYSTEM(UnitManager)
}
