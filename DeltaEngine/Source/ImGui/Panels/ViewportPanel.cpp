/**********************************************************************************
* \file   ViewportPanel.cpp
* \brief  The file contains implementation of ViewportPanel.
* \author Chin, Clara,   90% Code Contribution
* \author Ong, Graeme,   10% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ViewportPanel.h"
#include "Input/InputManager.h"
#include "Render/Camera.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Core/Utils/FileUtils.h"
#include "Input/Keys.h"
#include "Core/GlobalStruct.h"
#include "Core/Debugging/Gizmos.h"
#include "Core/GameClock/EngineClock.h"
#include "ImGui/Editor.h"
#include "Physics/Collision.h"

namespace DeltaEngine
{
  static Point curr_mouse{};
  static Point prev_mouse{};
  static bool dragging{false};

  void KeysInput()
  {
    auto delta = GetEnv().pClock->DeltaTime();
    auto speed = 10.0f;


    Camera::editorCamera->m_Size -= 2 * speed * ImGui::GetIO().MouseWheel * GetEnv().pClock->DeltaTime();

    if (ImGui::IsMouseReleased(0) && !dragging)
    {
      if (Editor::tool_selection == Editor::Tool::EntitySelector)
      {
        auto& em = GetEnv().pECS->GetWorld().GetEntityManager();
        std::vector<size_t> entities;

        em.ForEach([&](EntityID& id, Transform& t, Image& i)
        {
          if (CollisionIntersection_RectMouse(t.position, i.GetWorldSize(), curr_mouse))
            entities.push_back(id.index);
        });

        if (!entities.empty())
        {
          if (auto it = std::find(entities.begin(), entities.end(), Editor::entity_id); it != entities.end())
          {
            if (++it == entities.end())
            {
              Editor::entity_selected = true;
              Editor::entity_id = entities[0];
              Editor::selection_transform = em.GetComponent<Transform>({entities[0]});
              Editor::selection_transform.scale = {0.2f, 0.2f, 0.0f};
            }
            else
            {
              Editor::entity_selected = true;
              Editor::entity_id = *it;
              Editor::selection_transform = em.GetComponent<Transform>({*it});
              Editor::selection_transform.scale = {0.2f, 0.2f, 0.0f};
            }
          }
          else
          {
            Editor::entity_selected = true;
            Editor::entity_id = entities[0];
            Editor::selection_transform = em.GetComponent<Transform>({entities[0]});
            Editor::selection_transform.scale = {0.2f, 0.2f, 0.0f};
          }
        }
      }
    }
    else if (ImGui::IsMouseClicked(0))
    {
      if (Editor::entity_selected)
        if (CollisionIntersection_RectMouse(Editor::selection_transform.position, Editor::selection_transform.scale,
                                            curr_mouse))
          dragging = true;
    }
    else if (ImGui::IsMouseDown(0))
    {
      auto offset = curr_mouse - prev_mouse;

      if (Editor::tool_selection == Editor::Tool::Camera)
      {
        if (std::abs(offset.point_x) > FLT_EPSILON)
          Camera::editorCameraTransform.position.x -= offset.point_x;

        if (std::abs(offset.point_y) > FLT_EPSILON)
          Camera::editorCameraTransform.position.y -= offset.point_y;
      }
      else if (Editor::tool_selection == Editor::Tool::EntitySelector)
      {
        if (Editor::entity_selected)
          if (dragging)
          {
            Editor::selection_transform.position.x += offset.point_x;
            Editor::selection_transform.position.y += offset.point_y;
            auto& t = GetEnv().pECS->GetWorld().GetEntityManager().GetComponent<Transform>({Editor::entity_id});
            t.position.x = Editor::selection_transform.position.x;
            t.position.y = Editor::selection_transform.position.y;
          }
      }
    }
    else
    {
      if (ImGui::IsKeyDown(DEVK_W) && !ImGui::IsKeyDown(DEVK_S))
        Camera::editorCameraTransform.position.y += speed * delta;
      else if (ImGui::IsKeyDown(DEVK_S) && !ImGui::IsKeyDown(DEVK_W))
        Camera::editorCameraTransform.position.y -= speed * delta;
      if (ImGui::IsKeyDown(DEVK_A) && !ImGui::IsKeyDown(DEVK_D))
        Camera::editorCameraTransform.position.x -= speed * delta;
      else if (ImGui::IsKeyDown(DEVK_D) && !ImGui::IsKeyDown(DEVK_A))
        Camera::editorCameraTransform.position.x += speed * delta;
      if (ImGui::IsKeyDown(DEVK_Q) && !ImGui::IsKeyDown(DEVK_E))
        Camera::editorCamera->m_Size -= speed * delta;
      else if (ImGui::IsKeyDown(DEVK_E) && !ImGui::IsKeyDown(DEVK_Q))
        Camera::editorCamera->m_Size += speed * delta;
      dragging = false;
    }
  }


  ViewportPanel::ViewportPanel(std::string str, Editor& e) :
    IPanel(str, e)
  {
    m_enabled = true;
  }

  ViewportPanel::~ViewportPanel()
  {
    m_enabled = false;
  }

  void ViewportPanel::Render()
  {
    m_active = ImGui::Begin(m_name.c_str(), nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

    if (m_active)
    {
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ImVec2 renderPos = ImGui::GetCursorScreenPos(); // gives top left of the window
      ImVec2 renderSize = ImGui::GetContentRegionAvail(); // gives height and width 
      ren_pos = {renderPos.x, renderPos.y};
      ren_size = {renderSize.x, renderSize.y};


      if (ImGui::IsWindowHovered())
      {
        ImGui::SetWindowFocus();

        prev_mouse = curr_mouse;
        float cameraWidth = Camera::editorCamera->Max(Camera::editorCameraTransform).x - Camera::editorCamera->Min(
          Camera::editorCameraTransform).x;
        float cameraHeight = Camera::editorCamera->Max(Camera::editorCameraTransform).y - Camera::editorCamera->Min(
          Camera::editorCameraTransform).y;
        float cursorViewPortDistanceX = ImGui::GetMousePos().x - renderPos.x;
        float cursorViewPortDistanceY = ImGui::GetMousePos().y - renderPos.y;
        curr_mouse.point_x = ((cursorViewPortDistanceX / renderSize.x) * cameraWidth) + Camera::editorCamera->Min(
          Camera::editorCameraTransform).x;
        curr_mouse.point_y = Camera::editorCamera->Max(Camera::editorCameraTransform).y - ((cursorViewPortDistanceY /
          renderSize.y) * cameraHeight);
        std::cout << "curr_mouse.point_x is " << curr_mouse.point_x << std::endl;
        std::cout << "curr_mouse.point_y is " << curr_mouse.point_y << std::endl;
        KeysInput();
      }
      /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
      Camera::editorCamera->SetAspectRatio(viewportPanelSize.x, viewportPanelSize.y);
      Camera::editorCamera->SetViewportSize(viewportPanelSize.x);
      uint64_t textureID = Camera::editorCamera->GetFrameBuffer().GetColorAttachment();
      ImGui::Image(reinterpret_cast<void*>(textureID), viewportPanelSize, ImVec2{0, 1}, ImVec2{1, 0});

      if (ImGui::BeginDragDropTarget())
      {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("TILES"); payload)
        {
          std::string payload_n = *static_cast<std::string*>(payload->Data);

          // do the tiling
          EntityID tile = GetEnv().pECS->GetWorld().GetEntityManager().CreateEntity<Renderer2D, Image>();
          env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(tile).position = {
            curr_mouse.point_x, curr_mouse.point_y, 0
          };

          auto offset = payload_n.find_last_of('_');
          env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(tile).scale = {0.5, 0.5, 0.0};
          env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(tile).m_Sprite.m_Key = payload_n.
            substr(0, offset);
          env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(tile).m_Sprite.m_Index = std::stoi(
            payload_n.substr(offset + 1));
        }
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSETFILES"); payload)
        {
          std::string payload_n = *static_cast<std::string*>(payload->Data);
          if (payload_n.find(".png") != std::string::npos)
          {
            EntityID tile = GetEnv().pECS->GetWorld().GetEntityManager().CreateEntity<Renderer2D, Image>();

            env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(tile).position = {
              curr_mouse.point_x, curr_mouse.point_y, 0
            };
            env.pECS->GetWorld().GetEntityManager().GetComponent<Transform>(tile).scale = {0.5, 0.5, 0.0};
            env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(tile).m_Sprite.m_Key = payload_n.substr(
              0, payload_n.find_last_of('.'));
            env.pECS->GetWorld().GetEntityManager().GetComponent<Image>(tile).m_Sprite.m_Index = 0;
          }
        }

        ImGui::EndDragDropTarget();
      }
    }
    ImGui::End();
  }
}
