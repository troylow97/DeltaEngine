#include "RenderSystem.h"

namespace DeltaEngine
{
void RenderSystem::Update()
{
  RenderModule::openGLSystem->Update();

  Camera::editorCamera->Start();

  em.ForEach([&](EntityID id, Transform& t, Image& i, Renderer2D& r)
    {
      if (r.m_Active)
      {
      Vector2 offset = i.m_Offset + i.m_Sprite.GetOffset();
      Vector2 tiling = i.m_Tiling * i.m_Sprite.GetTiling();
      Vector2 pivot = i.m_Sprite.GetPivot();

      Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
      Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
      Matrix4x4 model = Matrix4x4::Scale(Vector3{
        (i.m_Sprite ? (i.m_Sprite.GetWidth() / 200.0f) : 1) * (i.m_FlipX ? -1 : 1),
        (i.m_Sprite ? (i.m_Sprite.GetHeight() / 200.0f) : 1) * (i.m_FlipY ? -1 : 1), 1 })
        * t.LocalToWorldMatrix();

      if (r.m_Shaded)
      {
        if (i.m_Sprite)
        {
          i.m_Sprite.GetTexture()->Bind(0);
        }

        r.m_Material.SetUniformMatrix4f("_M", model);
        r.m_Material.SetUniformMatrix4f("_V", view);
        r.m_Material.SetUniformMatrix4f("_P", proj);
        r.m_Material.SetUniformColor4f("_Color", r.color);
        r.m_Material.SetUniform1i("_MainTex", 0);
        Mesh::DrawQuad(offset, tiling, pivot);

        if (i.m_Sprite)
        {
          i.m_Sprite.GetTexture()->Unbind();
        }
      }
      if (r.m_Wireframe)
      {
        r.m_Material.SetUniformMatrix4f("_M", model);
        r.m_Material.SetUniformMatrix4f("_V", view);
        r.m_Material.SetUniformMatrix4f("_P", proj);
        Mesh::DrawQuad(true);
      }
      }
    });


  em.ForEach([&](EntityID id, Transform& t, Text& i, Renderer2D& r)
    {
      Matrix4x4 proj = Camera::editorCamera->GetProjectionMatrix();
      Matrix4x4 view = Camera::editorCamera->GetViewMatrix();
      Matrix4x4 model = t.LocalToWorldMatrix();

      // activate corresponding render state	
      r.m_Material.SetUniformMatrix4f("_M", model);
      r.m_Material.SetUniformMatrix4f("_V", view);
      r.m_Material.SetUniformMatrix4f("_P", proj);
      r.m_Material.SetUniformColor4f("_Color", r.color);
      r.m_Material.SetUniform1i("_MainTex", 0);

      if (r.m_Shaded)
        Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(i.m_FontKey), i.m_Text, 1, false);
      if (r.m_Wireframe)
        Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(i.m_FontKey), i.m_Text, 1, true);
    });

}
void RenderSystem::LateUpdate()
{
  Camera::editorCamera->End();

}
}
