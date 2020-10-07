#include "AnimationSystem.h"

namespace DeltaEngine
{
    
    void AnimationSystem::update()
    {
        em.for_each([&](EntityID id, Animator& a, SpriteRenderer& s)
            {
                a.Update();
                s.sprite = a.m_Clip->GetSprite(a.GetFrame());
            });
    }
    void AnimationSystem::late_update()
    {

    }
}
