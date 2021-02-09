/**********************************************************************************
* \file   RenderSystem.cpp
* \brief  The file contains implementation of RenderSystem.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "RenderSystem.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Profiler/Profiler.h"
#include "ECS/EntityManager.h"
#include "Render/Camera.h"
#include "Render/VideoClip.h"
#include "Render/Mesh.h"
#include "Render/Window.h"
#include "Render/OpenGLSystem.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
  Color wireframeColor = Color::Black();
  std::vector<EntityID> sortedRenderers2D;
  std::vector<EntityID> sortedRenderersOverlay;

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
    Material spriteMat{ "Shaders/DefaultSprite" };
    Material textMat{ "Shaders/DefaultText" };
    Material particleMat{ "Shaders/DefaultParticle" };

    Matrix4x4 proj = c.GetProjectionMatrix(tr);
    Matrix4x4 view = c.GetViewMatrix(tr);
    
    for (EntityID ID : sortedRenderers2D)
    {
      if (!em.HasComponent<Transform>(ID) || !em.HasComponent<Renderer2D>(ID))
        continue;
      Transform& t = em.GetComponent<Transform>(ID);
      Renderer2D& r = em.GetComponent<Renderer2D>(ID);
      if (em.HasComponent<Image>(ID))
      {
        Image& img = em.GetComponent<Image>(ID);

        if (r.m_Active)
        {
          Vector2 offset = img.m_Offset + img.m_Sprite.GetOffset();
          Vector2 tiling = img.m_Tiling * img.m_Sprite.GetTiling();
          Vector2 pivot = img.m_Sprite.GetPivot();

          Matrix4x4 model = Matrix4x4::Scale(Vector3{
              (img.m_Sprite ? (img.m_Sprite.GetWidth() / 200.0f) : 1) * img.m_Size.x * (img.m_FlipX ? -1 : 1),
              (img.m_Sprite ? (img.m_Sprite.GetHeight() / 200.0f) : 1) * img.m_Size.y * (img.m_FlipY ? -1 : 1), 1
            })
            * t.LocalToWorldMatrix();

          if (r.m_Shaded)
          {
            if (img.m_Sprite)
            {
              img.m_Sprite.GetTexture()->Bind();
            }

            spriteMat.SetUniformMatrix4f("_M", model);
            spriteMat.SetUniformMatrix4f("_V", view);
            spriteMat.SetUniformMatrix4f("_P", proj);
            spriteMat.SetUniformColor4f("_Color", r.m_Color);
            spriteMat.SetUniform1i("_MainTex", 0);
            spriteMat.SetUniform1i("_FillType", static_cast<int>(img.m_FillType));
            spriteMat.SetUniform1f("_FillAmount", img.m_FillAmount);
            spriteMat.SetUniform1f("_RRot", img.m_OverallAngle);
            spriteMat.SetUniform1f("_RStart", img.m_StartAngle);
            spriteMat.SetUniform1f("_REnd", img.m_EndAngle);
            spriteMat.SetUniformVector4f("_SpriteUV", Vector4(
              img.m_Sprite.GetOffset().x,
              img.m_Sprite.GetOffset().y,
              img.m_Sprite.GetOffset().x + img.m_Sprite.GetTiling().x,
              img.m_Sprite.GetOffset().y + img.m_Sprite.GetTiling().y));

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            Mesh::DrawQuad(offset, tiling, pivot);

            if (img.m_Sprite)
            {
              img.m_Sprite.GetTexture()->Unbind();
            }
          }
          if (r.m_Wireframe)
          {
            spriteMat.SetUniformMatrix4f("_M", model);
            spriteMat.SetUniformMatrix4f("_V", view);
            spriteMat.SetUniformMatrix4f("_P", proj);
            spriteMat.SetUniform1i("_FillType", 0);
            spriteMat.SetUniformColor4f("_Color", wireframeColor);
            Mesh::DrawQuad(true);
          }
        }
      }
      if (em.HasComponent<Text>(ID))
      {
        Text& x = em.GetComponent<Text>(ID);
        glClear(GL_DEPTH_BUFFER_BIT);
        Matrix4x4 model = t.LocalToWorldMatrix();

        // activate corresponding render state	
        textMat.SetUniformMatrix4f("_M", model);
        textMat.SetUniformMatrix4f("_V", view);
        textMat.SetUniformMatrix4f("_P", proj);
        textMat.SetUniform1i("_MainTex", 0);

        if (r.m_Shaded)
        {
          glEnable(GL_BLEND);
          glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
          glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

          textMat.SetUniformColor4f("_Color", r.m_Color);
          Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(x.m_FontKey), x.m_Text, 1, false);
        }
        if (r.m_Wireframe)
        {
          textMat.SetUniformColor4f("_Color", wireframeColor);
          Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(x.m_FontKey), x.m_Text, 1, true);
        }
      }
      if (em.HasComponent<ParticleEmitter>(ID))
      {
        ParticleEmitter& ps = em.GetComponent<ParticleEmitter>(ID);

        Matrix4x4 model = t.LocalToWorldMatrix();

        GetEnv().pManager->Get<Texture2D>("Textures/DefaultParticle")->Bind();
        particleMat.SetUniformMatrix4f("_M", model);
        particleMat.SetUniformMatrix4f("_V", view);
        particleMat.SetUniformMatrix4f("_P", proj);
        particleMat.SetUniform1i("_MainTex", 0);

        std::vector<float> locationSizes { 4, 4, 4 };
        std::vector<float> particleMats;
        for (auto& particle : ps.particlePools[ID])
        {
          if (!particle.active)
            continue;
          Matrix4x4 pMat = Matrix4x4::Transpose(particle.transform.LocalToWorldMatrix());

          //vertex position
          for (unsigned int j = 0; j < 12; ++j)
          {
            particleMats.push_back(pMat.m[j]);
          }
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        Mesh::DrawQuadInst(ps.m_ActiveParticles, particleMats, locationSizes);
      }
    }
  }

  void DrawRendererOverlay(EntityManager& em, Camera& c)
  {
    for (EntityID ID : sortedRenderersOverlay)
    {
      if (!em.HasComponent<Transform>(ID) || !em.HasComponent<RendererOverlay>(ID))
        continue;
      if (em.HasComponent<Image>(ID))
      {
        Transform t{};
        RendererOverlay& r = em.GetComponent<RendererOverlay>(ID);
        Image& i = em.GetComponent<Image>(ID);
        float refAspect = r.refRes.x / r.refRes.y;
        float cw = r.refRes.x;
        float ch = r.refRes.y;

        if (r.m_Active)
        {
          Vector2 offset = i.m_Offset + i.m_Sprite.GetOffset();
          Vector2 tiling = i.m_Tiling * i.m_Sprite.GetTiling();
          Vector2 pivot = r.pivot;

          Matrix4x4 scale = Matrix4x4::Scale(Vector3(1, 1, 1));
          float tmpXscale = 1;
          float tmpYscale = 1;

          r.anchorMin.x = Math::Clamp01(r.anchorMin.x);
          r.anchorMin.y = Math::Clamp01(r.anchorMin.y);
          r.anchorMax.x = Math::Clamp01(r.anchorMax.x);
          r.anchorMax.y = Math::Clamp01(r.anchorMax.y);

          if (Math::Abs(r.anchorMax.x - r.anchorMin.x) > .01f)
          {
            float anch = (r.anchorMax.x - r.anchorMin.x) / 2 - .5f;
            float midpt = (r.left - r.right) / 2 / cw;
            t.position.x = anch + midpt;
            tmpXscale = (1 - (r.left + r.right) / cw);
            pivot = Vector2(.5f, .5f);
          }
          else
          {
            t.position.x = r.pos.x / cw + r.anchorMin.x - .5f;
            tmpXscale = r.size.x / cw;
          }
          if (Math::Abs(r.anchorMax.y - r.anchorMin.y) > .01f)
          {
            float anch = (r.anchorMax.y - r.anchorMin.y) / 2 - .5f;
            float midpt = (r.bottom - r.top) / 2 / ch;
            t.position.y = anch + midpt;
            tmpYscale = (1 - (r.top + r.bottom) / ch);
            pivot = Vector2(.5f, .5f);
          }
          else
          {
            t.position.y = r.pos.y / ch + r.anchorMin.y - .5f;
            tmpYscale = r.size.y / ch;
          }

          if (r.m_PreserveAspect)
          {
            float sprAspect = 1.0f * i.m_Sprite.GetWidth() / i.m_Sprite.GetHeight();

            if (tmpXscale / tmpYscale > sprAspect)
            {
              tmpXscale = tmpYscale * sprAspect / refAspect;
            }
            else
            {
              tmpYscale = tmpXscale / sprAspect * refAspect;
            }
          }

          scale *= Matrix4x4::Scale(Vector3(tmpXscale, tmpYscale, 1));

          Matrix4x4 proj = Matrix4x4::identity;
          Matrix4x4 view = Matrix4x4::Scale(2) * Matrix4x4::identity;
          Matrix4x4 model = scale * t.LocalToWorldMatrix();


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
            r.m_Material.SetUniform1f("_FadeAmt", 0);
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
        Transform& t = em.GetComponent<Transform>(ID);
        RendererOverlay& r = em.GetComponent<RendererOverlay>(ID);
        Text& x = em.GetComponent<Text>(ID);
        glClear(GL_DEPTH_BUFFER_BIT);
        Matrix4x4 proj = Matrix4x4::identity;
        Matrix4x4 view = Matrix4x4::identity;
        Matrix4x4 model = t.LocalToWorldMatrix();

        // activate corresponding render state	
        r.m_Material.SetUniformMatrix4f("_M", model);
        r.m_Material.SetUniformMatrix4f("_V", view);
        r.m_Material.SetUniformMatrix4f("_P", proj);
        r.m_Material.SetUniform1f("_FadeAmt", 0);
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
  VideoClip* video = nullptr;
  void RenderSystem::Update()
  {
    RenderModule::openGLSystem->Update();

    // sort renderers by layer
    sortedRenderers2D.clear();
    em.ForEach(e_query, [&](EntityID id, Renderer2D& r) { sortedRenderers2D.push_back(id); });
    std::sort(sortedRenderers2D.begin(), sortedRenderers2D.end(), SortSprites);

    sortedRenderersOverlay.clear();
    em.ForEach(e_query, [&](EntityID id, RendererOverlay& r) { sortedRenderersOverlay.push_back(id); });
    std::sort(sortedRenderersOverlay.begin(), sortedRenderersOverlay.end(), SortSprites);

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
        DrawRendererOverlay(em, c);

        c.End();

#ifndef DE_EDITOR
        Shader* shader = GetEnv().pManager->Get<Shader>("DefaultScreen");
        shader->SetUniform1i("_MainTex", 0);
        glBindTexture(GL_TEXTURE_2D, c.GetFrameBuffer().GetColorAttachment());

        Mesh::DrawQuad();
#endif // !DE_EDITOR
      });
    if (Camera::allCameras.size())
    {
      Camera::finalFrameBuffer->Resize(static_cast<unsigned int>(Camera::allCameras[0]->GetTrueViewportSize()),
        static_cast<unsigned int>(Camera::allCameras[0]->GetTrueViewportSize() / Camera::allCameras[0]->GetAspectRatio()));
    }

    Camera::finalFrameBuffer->Bind();

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT /*| GL_DEPTH_BUFFER_BIT*/);

    em.ForEach([&](EntityID id, Transform& tr, Camera& c)
      {
        Vector2 cameraAspect{ 1, 1 };

        if (Camera::GetFixedAspectRatio() > .01f)
        {
          if (c.GetAspectRatio() > Camera::GetFixedAspectRatio())
          {
            cameraAspect.x = Camera::GetFixedAspectRatio() / c.GetAspectRatio();
          }
          else
          {
            cameraAspect.y = c.GetAspectRatio() / Camera::GetFixedAspectRatio();
          }
        }

        // cameraAspect may be modified further depending on advanced camera settings

        Shader* shader = GetEnv().pManager->Get<Shader>("DefaultScreen");
        shader->SetUniform1i("_MainTex", 0);
        shader->SetUniformVector2f("_ScreenAspect", cameraAspect);
        glBindTexture(GL_TEXTURE_2D, c.GetFrameBuffer().GetColorAttachment());

        Mesh::DrawQuad();
      });

    Camera::finalFrameBuffer->Unbind();

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