#include "RenderSystem.h"

namespace DeltaEngine
{
void RenderSystem::Update()
{
  RenderModule::openGLSystem->Update();
  em.ForEach([&](EntityID id, Transform& c, Renderer& r, SpriteRenderer& s)
    {
      
    });

  Camera::editorCamera->Start();
}
void RenderSystem::LateUpdate()
{
  Camera::editorCamera->End();

}
}
