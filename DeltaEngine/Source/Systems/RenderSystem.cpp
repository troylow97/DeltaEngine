#include "RenderSystem.h"

namespace DeltaEngine
{
    void RenderSystem::Update()
    {
        RenderModule::openGLSystem->Update();

        Camera::editorCamera->Start();
    }
    void RenderSystem::LateUpdate()
    {
        Camera::editorCamera->End();

    }
}
