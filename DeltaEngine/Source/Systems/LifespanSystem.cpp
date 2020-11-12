#include "LifespanSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"

namespace DeltaEngine
{
    void LifespanSystem::Update()
    {
        em.ForEach([&](EntityID& id, Lifespan& ls)
        {
            if (ls.Lifespan < 0)
            {
                em.DestroyEntity(id);
            }
            else
            {
                ls.Lifespan -= env.pClock->DeltaTime();
            }
        });
    }
    void LifespanSystem::LateUpdate()
    {

    }

}
