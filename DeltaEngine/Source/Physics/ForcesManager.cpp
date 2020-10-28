#include "ForcesManager.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
    void ForcesManager::Update()
    {
        for (Force f : ForcesList)
        {
            if(f.isActive)
            {
                f.Age += GetEnv().pClock->DeltaTime();
                f.Magnitude *= (f.Lifetime - f.Age);
            }
        }
    }

    void ForcesManager::LateUpdate()
    {

    }

    void ForcesManager::RegisterForce(Force force)
    {
        ForcesList.push_back(force);
    }
}