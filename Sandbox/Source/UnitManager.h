#pragma once
#include "ECS/Entities.h"

struct UnitManager
{
    DeltaEngine::EntityID player;

    void Initialize();
  
};

extern UnitManager unit_manager;

UnitManager& GetUnitManager();