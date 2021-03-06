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
#include "Components/ParticleEmitter.h"
#include "Core/Utils/Random.h"

namespace DeltaEngine
{
  Color wireframeColor = Color::Black();
  std::vector<EntityID> sortedRenderers2D;
  std::vector<EntityID> sortedRenderersOverlay;
  const unsigned NO_SPRITE_INDEX = unsigned(~0);

  bool SortSprites(EntityID a, EntityID b)
  {
    Renderer2D& i = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(a);
    Renderer2D& j = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<Renderer2D>(b);
    return
      (i.m_SortingLayer < j.m_SortingLayer) ||
      (i.m_SortingLayer == j.m_SortingLayer) &&
      (i.m_SortingOrder < j.m_SortingOrder);
  }

  bool SortOverlay(EntityID a, EntityID b)
  {
    RendererOverlay& i = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<RendererOverlay>(a);
    RendererOverlay& j = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<RendererOverlay>(b);
    return
      (i.m_SortingLayer < j.m_SortingLayer) ||
      (i.m_SortingLayer == j.m_SortingLayer) &&
      (i.m_SortingOrder < j.m_SortingOrder);
  }

  void DrawRenderer2D(EntityManager& em, Camera& c, Transform& tr)
  {
    Material defaultMat{ "Shaders/Default" };
    Material spriteMat{ "Shaders/DefaultSprite" };
    Material textMat{ "Shaders/DefaultText" };
    Material particleMat{ "Shaders/DefaultParticle" };

    Matrix4x4 proj = c.GetProjectionMatrix(tr);
    Matrix4x4 view = c.GetViewMatrix(tr);

    std::vector<float> spriteLocationSizes{ 4, 4, 4, 4, 1, 1, 4, 1, 1, 1 };
    std::vector<float> spriteInstProps;
    unsigned currentTextureID = NO_SPRITE_INDEX; // treating ~0 as no sprite
    unsigned batches = 0;
    unsigned batchCount = 0;
    bool batchingSprites = false;

    auto DrawSpriteBatch = [&]()
    {
      // need to flush and start a new batch
      // draw the previous batch of sprites
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

      if (currentTextureID != NO_SPRITE_INDEX)
      {
        Texture2D::BindID(currentTextureID);
      }
      spriteMat.Bind();
      spriteMat.SetUniformMatrix4f("_M", Matrix4x4::identity);
      spriteMat.SetUniformMatrix4f("_V", view);
      spriteMat.SetUniformMatrix4f("_P", proj);
      spriteMat.SetUniform1i("_MainTex", 0);

      Mesh::DrawQuadInst(batchCount, spriteInstProps, spriteLocationSizes);

      ++batches;
    };
    
    for (EntityID ID : sortedRenderers2D)
    {
      if (!em.HasComponent<Transform>(ID) || !em.HasComponent<Renderer2D>(ID))
        continue;
      Transform& t = em.GetComponent<Transform>(ID);
      Renderer2D& r = em.GetComponent<Renderer2D>(ID);
      if (!r.m_Active)
        continue;
      if (em.HasComponent<Image>(ID))
      {
        Image& img = em.GetComponent<Image>(ID);

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
          // first check if a batch has started
          if (batchingSprites)
          {
            // then check if the current batch shares the same texture
            if ((img.m_Sprite && currentTextureID != img.m_Sprite.GetTexture()->GetRendererID()) ||
              (!img.m_Sprite && currentTextureID != NO_SPRITE_INDEX))
            {
              DrawSpriteBatch();

              // then reset vars appropriately
              currentTextureID = img.m_Sprite ? img.m_Sprite.GetTexture()->GetRendererID() : NO_SPRITE_INDEX;
              spriteInstProps.clear();
              batchCount = 0;
            }
          }
          else
          {
            // start a new batch
            batchingSprites = true;
            currentTextureID = img.m_Sprite ? img.m_Sprite.GetTexture()->GetRendererID() : NO_SPRITE_INDEX;
            spriteInstProps.clear();
            batchCount = 0;
            // then add data as normal
          }

          Matrix4x4 sMat = Matrix4x4::Transpose(model);
          //vertex position
          for (unsigned j = 0; j < 12; ++j)
          {
            spriteInstProps.push_back(sMat.m[j]);
          }
          spriteInstProps.push_back(r.m_Color.r);
          spriteInstProps.push_back(r.m_Color.g);
          spriteInstProps.push_back(r.m_Color.b);
          spriteInstProps.push_back(r.m_Color.a);
          spriteInstProps.push_back(static_cast<float>(img.m_FillType));
          spriteInstProps.push_back(img.m_FillAmount);
          spriteInstProps.push_back(offset.x);
          spriteInstProps.push_back(offset.y);
          spriteInstProps.push_back(offset.x + tiling.x);
          spriteInstProps.push_back(offset.y + tiling.y);
          spriteInstProps.push_back(img.m_OverallAngle);
          spriteInstProps.push_back(img.m_StartAngle);
          spriteInstProps.push_back(img.m_EndAngle);
          
          ++batchCount;
        }
        if (r.m_Wireframe)
        {
          defaultMat.SetUniformMatrix4f("_M", model);
          defaultMat.SetUniformMatrix4f("_V", view);
          defaultMat.SetUniformMatrix4f("_P", proj);
          defaultMat.SetUniform1i("_FillType", 0);
          defaultMat.SetUniformColor4f("_Color", wireframeColor);
          Mesh::DrawQuad(offset, tiling, pivot, true);
        }
      }
      if (em.HasComponent<Text>(ID))
      {
        if (batchingSprites)
        {
          DrawSpriteBatch();
          batchingSprites = false;
        }
        
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
        if (batchingSprites)
        {
          DrawSpriteBatch();
          batchingSprites = false;
        }
        ParticleEmitter& ps = em.GetComponent<ParticleEmitter>(ID);

        Matrix4x4 model = t.LocalToWorldMatrix();

        GetEnv().pManager->Get<Texture2D>("Textures/DefaultParticle")->Bind();
        particleMat.SetUniformMatrix4f("_M", model);
        particleMat.SetUniformMatrix4f("_V", view);
        particleMat.SetUniformMatrix4f("_P", proj);
        particleMat.SetUniform1i("_MainTex", 0);
        particleMat.SetUniformColor4f("_Color", r.m_Color);

        std::vector<float> locationSizes { 4, 4, 4, 4 };
        std::vector<float> particleProps;
        for (auto& particle : ps.particlePools[ID])
        {
          if (!particle.active)
            continue;
          Matrix4x4 pMat = Matrix4x4::Transpose(particle.transform.LocalToWorldMatrix());

          //vertex position
          for (unsigned j = 0; j < 12; ++j)
          {
            particleProps.push_back(pMat.m[j]);
          }
          particleProps.push_back(particle.color.r);
          particleProps.push_back(particle.color.g);
          particleProps.push_back(particle.color.b);
          particleProps.push_back(particle.color.a);
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        Mesh::DrawQuadInst(ps.m_ActiveParticles, particleProps, locationSizes);
      }
    }
    if (batchingSprites)
    {
      DrawSpriteBatch();
      batchingSprites = false;
    }
    //std::cerr << "Batches = " << batches << std::endl;
    //std::cerr << "FPS = " << 1 / DeltaTime() << std::endl;
  }

  void DrawRendererOverlay(EntityManager& em, Camera& c)
  {
    Material spriteMat{ "Shaders/Default" };
    Material textMat{ "Shaders/DefaultText" };
    
    for (EntityID ID : sortedRenderersOverlay)
    {
      if (!em.HasComponent<Transform>(ID) || !em.HasComponent<RendererOverlay>(ID))
        continue;
      RendererOverlay& r = em.GetComponent<RendererOverlay>(ID);
      if (r.m_Active)
        continue;
      if (em.HasComponent<Image>(ID))
      {
        Transform t{};
        Image& img = em.GetComponent<Image>(ID);
        float refAspect = r.refRes.x / r.refRes.y;
        float cw = r.refRes.x;
        float ch = r.refRes.y;

        Vector2 offset = img.m_Offset + img.m_Sprite.GetOffset();
        Vector2 tiling = img.m_Tiling * img.m_Sprite.GetTiling();
        Vector2 pivot = r.pivot;

        Matrix4x4 scale = Matrix4x4::identity;
        float tmpXscale = 1;
        float tmpYscale = 1;

        // screen space math
        {
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
            float sprAspect = 1.0f * img.m_Sprite.GetWidth() / img.m_Sprite.GetHeight();

            if (tmpXscale / tmpYscale > sprAspect)
            {
              tmpXscale = tmpYscale * sprAspect / refAspect;
            }
            else
            {
              tmpYscale = tmpXscale / sprAspect * refAspect;
            }
          }
        }

        scale *= Matrix4x4::Scale(Vector3(tmpXscale, tmpYscale, 1));

        Matrix4x4 proj = Matrix4x4::identity;
        Matrix4x4 view = Matrix4x4::Scale(2);
        Matrix4x4 model = scale * t.LocalToWorldMatrix();

        if (r.m_Shaded)
        {
          if (img.m_Sprite)
          {
            img.m_Sprite.GetTexture()->Bind(0);
          }

          spriteMat.SetUniformMatrix4f("_M", model);
          spriteMat.SetUniformMatrix4f("_V", view);
          spriteMat.SetUniformMatrix4f("_P", proj);
          spriteMat.SetUniformColor4f("_Color", r.m_Color);
          spriteMat.SetUniform1i("_MainTex", 0);
          spriteMat.SetUniform1f("_FadeAmt", 0);
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
          Mesh::DrawQuad(offset, tiling, pivot, true);
        }
      }
      if (em.HasComponent<Text>(ID))
      {
        Transform& t = em.GetComponent<Transform>(ID);
        Text& x = em.GetComponent<Text>(ID);
        glClear(GL_DEPTH_BUFFER_BIT);
        Matrix4x4 proj = Matrix4x4::identity;
        Matrix4x4 view = Matrix4x4::identity;
        Matrix4x4 model = t.LocalToWorldMatrix();

        // activate corresponding render state	
        textMat.SetUniformMatrix4f("_M", model);
        textMat.SetUniformMatrix4f("_V", view);
        textMat.SetUniformMatrix4f("_P", proj);
        textMat.SetUniform1f("_FadeAmt", 0);
        textMat.SetUniform1i("_MainTex", 0);

        if (r.m_Shaded)
        {
          textMat.SetUniformColor4f("_Color", r.m_Color);
          Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(x.m_FontKey), x.m_Text, 1, false);
        }
        if (r.m_Wireframe)
        {
          textMat.SetUniformColor4f("_Color", wireframeColor);
          Mesh::DrawTextMesh(GetEnv().pManager->Get<Font>(x.m_FontKey), x.m_Text, 1, true);
        }
      }
    }
  }

  void RenderSystem::Update()
  {
    RenderModule::openGLSystem->Update();

    // sort renderers by layer
    sortedRenderers2D.clear();
    em.ForEach(e_query, [&](EntityID id, Renderer2D& r) { sortedRenderers2D.push_back(id); });
    std::sort(sortedRenderers2D.begin(), sortedRenderers2D.end(), SortSprites);

    sortedRenderersOverlay.clear();
    em.ForEach(e_query, [&](EntityID id, RendererOverlay& r) { sortedRenderersOverlay.push_back(id); });
    std::sort(sortedRenderersOverlay.begin(), sortedRenderersOverlay.end(), SortOverlay);

    // camera entities
    em.ForEach([&](EntityID id, Transform& tr, Camera& c)
      {
#ifndef DE_EDITOR
        c.SetViewportSize(1.0f * GetEnv().pWin->Width());
        c.SetAspectRatio(1.0f * GetEnv().pWin->Width(), 1.0f * GetEnv().pWin->Height());
#endif // !DE_EDITOR

        if (c.shakeMagnitude > 0)
        {
          tr.position -= c.shakeDisp;

          c.shakeDisp = Vector2(
            Random::RandomFloatRange(-.5f, .5f) * c.shakeMagnitude / 10,
            Random::RandomFloatRange(-.5f, .5f) * c.shakeMagnitude / 10);

          tr.position += c.shakeDisp;

          c.shakeMagnitude -= static_cast<float>(DeltaTime()) * c.shakeFade;
          if (c.shakeMagnitude < 0)
            c.shakeMagnitude = 0;
        }
        Matrix4x4 proj = c.GetProjectionMatrix(tr);

        for (size_t i = 0; i < 16; i++)
        {
          std::cerr << proj.m[i] << ",";
        }
        std::cerr << std::endl;

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
      Camera::finalFrameBuffer->Resize(static_cast<unsigned>(Camera::allCameras[0]->GetTrueViewportSize()),
        static_cast<unsigned>(Camera::allCameras[0]->GetTrueViewportSize() / Camera::allCameras[0]->GetAspectRatio()));
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
