#include "LifespanSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/GameClock/GameClock.h"

namespace DeltaEngine
{
    void LifespanSystem::Update()
    {
        em.ForEach([&](EntityID& id, Lifespan& ls)
        {
            if (ls.Timer < 0)
            {
                em.DestroyEntity(id);             
            }
            else
            {
                ls.Timer -= env.pClock->DeltaTime();
            }
        });

        em.ForEach([&](EntityID& id, Health& hp)
            {
                if (hp.CurrentHealth < 0)
                {
                    em.DestroyEntity(id);
                }
            });

    }
    void LifespanSystem::LateUpdate()
    {

    }

}
