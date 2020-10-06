#pragma once

#include "DeltaEngine.h"

namespace DeltaEngine
{
    DEFINE_SYSTEM(RenderSystem, Transform, Renderer)

    void update() override
    {
        //std::cout << "\n\nRenderSystem Update" << std::endl;
        //std::cout << "Nothing to do here" << std::endl;

        // animation
        em.for_each(e_query, [&](EntityID id, SpriteRenderer& s, Transform& t, FrameAnimation& f)
            {

            });

        // camera
        Camera::editorCamera->Start();
        em.for_each(e_query, [&](EntityID id, Renderer& r)
            {
                r.Render(Camera::editorCamera);
            });
        //Gizmos stuff here
        Camera::editorCamera->End();

        RenderModule::openGLSystem->Update();
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
