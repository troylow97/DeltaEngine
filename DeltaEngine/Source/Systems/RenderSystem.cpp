#include "RenderSystem.h"

namespace DeltaEngine
{
    void RenderSystem::update()
    {
        RenderModule::openGLSystem->Update();

        Camera::editorCamera->Start();
    }
    void RenderSystem::late_update()
    {
        Camera::editorCamera->End();

        ::SwapBuffers(RenderModule::openGLSystem->GetWindowContext());
    }
}
