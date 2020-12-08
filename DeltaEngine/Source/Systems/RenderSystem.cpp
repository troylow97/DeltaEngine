#include "RenderSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "ECS/EntityManager.h"
#include "Render/Camera.h"
#include "Render/Mesh.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
  Color wireframeColor = Color::Black();
  std::vector<EntityID> sortedRenderers;

  bool SortSprites(EntityID a, EntityID b)
  {
    Renderer2D& i = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(a);
    Renderer2D& j = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(b);
    return
      (i.m_SortingLayer < j.m_SortingLayer) ||
      (i.m_SortingLayer == j.m_SortingLayer) &&
      (i.m_SortingOrder < j.m_SortingOrder);
  }

  void DrawRenderer2D(EntityManager& em, Camera& c, Transform& tr)
  {
    for (EntityID ID : sortedRenderers)
    {
      if (!em.HasComponent<Transform>(ID) || !em.HasComponent<Renderer2D>(ID))
        continue;
      Transform& t = em.GetComponent<Transform>(ID);
      Renderer2D& r = em.GetComponent<Renderer2D>(ID);
      if (em.HasComponent<Image>(ID))
      {
        Image& i = em.GetComponent<Image>(ID);

        if (r.m_Active)
        {
          Vector2 offset = i.m_Offset + i.m_Sprite.GetOffset();
          Vector2 tiling = i.m_Tiling * i.m_Sprite.GetTiling();
          Vector2 pivot = i.m_Sprite.GetPivot();

          Matrix4x4 proj = c.GetProjectionMatrix(tr);
          Matrix4x4 view = c.GetViewMatrix(tr);
          Matrix4x4 model = Matrix4x4::Scale(Vector3{
              (i.m_Sprite ? (i.m_Sprite.GetWidth() / 200.0f) : 1) * i.m_Size.x * (i.m_FlipX ? -1 : 1),
              (i.m_Sprite ? (i.m_Sprite.GetHeight() / 200.0f) : 1) * i.m_Size.y * (i.m_FlipY ? -1 : 1), 1
            })
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
            r.m_Material.SetUniformColor4f("_Color", r.m_Color);
            r.m_Material.SetUniform1i("_MainTex", 0);
            r.m_Material.SetUniform1f("_FadeAmt", c.fadeColorAmt);
            r.m_Material.SetUniform1i("_FillType", static_cast<int>(i.m_FillType));
            r.m_Material.SetUniform1f("_FillAmount", i.m_FillAmount);
            r.m_Material.SetUniform1f("_RRot", i.m_OverallAngle);
            r.m_Material.SetUniform1f("_RStart", i.m_StartAngle);
            r.m_Material.SetUniform1f("_REnd", i.m_EndAngle);
            r.m_Material.SetUniformVector4f("_SpriteUV", Vector4(
              i.m_Sprite.GetOffset().x,
              i.m_Sprite.GetOffset().y,
              i.m_Sprite.GetOffset().x + i.m_Sprite.GetTiling().x,
              i.m_Sprite.GetOffset().y + i.m_Sprite.GetTiling().y));
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
            r.m_Material.SetUniform1i("_FillType", 0);
            r.m_Material.SetUniformColor4f("_Color", wireframeColor);
            Mesh::DrawQuad(true);
          }
        }
      }
      if (em.HasComponent<Text>(ID))
      {
        Text& x = em.GetComponent<Text>(ID);
        glClear(GL_DEPTH_BUFFER_BIT);
        Matrix4x4 proj = c.GetProjectionMatrix(tr);
        Matrix4x4 view = c.GetViewMatrix(tr);
        Matrix4x4 model = t.LocalToWorldMatrix();

        // activate corresponding render state	
        r.m_Material.SetUniformMatrix4f("_M", model);
        r.m_Material.SetUniformMatrix4f("_V", view);
        r.m_Material.SetUniformMatrix4f("_P", proj);
        r.m_Material.SetUniform1f("_FadeAmt", c.fadeColorAmt);
        r.m_Material.SetUniform1i("_MainTex", 0);

        if (r.m_Shaded)
        {
          r.m_Material.SetUniformColor4f("_Color", r.m_Color);
          Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(x.m_FontKey), x.m_Text, 1, false);
        }
        if (r.m_Wireframe)
        {
          r.m_Material.SetUniformColor4f("_Color", wireframeColor);
          Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(x.m_FontKey), x.m_Text, 1, true);
        }
      }
    }
  }

  void RenderSystem::Update()
  {
    RenderModule::openGLSystem->Update();

    // sort renderers by layer
    sortedRenderers.clear();
    em.ForEach(e_query, [&](EntityID id, Renderer2D& r) { sortedRenderers.push_back(id); });
    std::sort(sortedRenderers.begin(), sortedRenderers.end(), SortSprites);

    // camera entities
    em.ForEach([&](EntityID id, Transform& tr, Camera& c)
      {
#ifndef DE_EDITOR
        c.SetViewportSize(1.0f * GetEnv().pWin->Width());
        c.SetAspectRatio(1.0f * GetEnv().pWin->Width(), 1.0f * GetEnv().pWin->Height());
#endif // !DE_EDITOR

        c.Start();
        // loop through every object
        DrawRenderer2D(em, c, tr);

        c.End();

#ifndef DE_EDITOR
        Shader* shader = GetEnv().pManager->Get<Shader>("DefaultScreen");
        shader->SetUniform1i("_MainTex", 0);
        glBindTexture(GL_TEXTURE_2D, c.GetFrameBuffer().GetColorAttachment());
        Mesh::DrawQuad();
#endif // !DE_EDITOR
      });


#ifdef DE_EDITOR
    Camera::editorCamera->Start();
    DrawRenderer2D(em, *Camera::editorCamera, Camera::editorCameraTransform);
#endif // DE_EDITOR

    Profiler::Instance().Record("Render System Update");
  }

  void RenderSystem::LateUpdate()
  {
#ifdef DE_EDITOR
    Camera::editorCamera->End();
#endif // DE_EDITOR

    Profiler::Instance().Record("Render System Late Update");
  }
}