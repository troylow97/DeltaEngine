#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(AnimationSystem, Transform, Animator, SpriteRenderer)

    void update() override
    {
        //std::cout << "\n\nRenderSystem Update" << std::endl;
        //std::cout << "Nothing to do here" << std::endl;

        // animation
        em.for_each(e_query, [&](EntityID id, SpriteRenderer& s, Animator& a)
            {
                a.timer += static_cast<float>(DeltaTime());
                if (a.timer > a.animation->GetTotalFrames() / a.animation->GetFps())
                    a.timer -= a.animation->GetTotalFrames() / a.animation->GetFps();

                a.frame = (unsigned int)(a.timer * a.animation->GetFps());

                s.sprite = a.animation->GetSprite(a.frame);

            });
    }

    void late_update() override
    {
        //std::cout << "\n\nRenderSystem Late Update" << std::endl;
        //em.for_each(e_query, [](DeltaEngine::EntityID id, Transform& t) {
        //    std::cout << "Entity Index " << id.index << std::endl;
        //    std::cout << "Rendered Position " << t.position << std::endl;
        //    });
    }

    END_DEFINE_SYSTEM(RenderSystem)
}
