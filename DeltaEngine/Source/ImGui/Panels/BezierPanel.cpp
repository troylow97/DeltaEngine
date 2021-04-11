/**********************************************************************************
* \file   BezierPanel.cpp
* \brief  The file contains implementation of BezierPanel.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "BezierPanel.h"

#include "DeltaEngine.h"
#include "imgui.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "ImGui/ImGuiMath.h"
#include "Input/Keys.h"

#include <string>

namespace DeltaEngine
{
  BezierPanel::BezierPanel(std::string str, Editor& e)
    : IPanel(str, e)
  {
    m_enabled = false;
  }

  BezierPanel::~BezierPanel()
  {
    m_enabled = false;
  }

  void BezierPanel::Enable()
  {
    m_enabled = !m_enabled;
    loaded = false;
  }

  void BezierPanel::Render()
  {
    if (ImGui::Begin("Bezier Curve Editor", &m_enabled,
      ImGuiWindowFlags_MenuBar |
      ImGuiWindowFlags_NoNavInputs |
      ImGuiWindowFlags_NoDocking))
    {
      static int anchorDragged = -1;
      static int controlDraggedL = -1;
      static int controlDraggedR = -1;
      static int selectedKey = -1;
      static int selectedCurve = -1;
      static ImVec2 keyPropsPos = ImVec2(0.f, 0.f);
      static bool draggingKeyProps = false;
      static bool hoveringKeyProps = false;

      ImGuiIO& io = ImGui::GetIO();
      ImColor cols[] =
      {
        {100, 17, 17, 255},
        {17, 100, 17, 255},
        {17, 17, 100, 255},
        {17, 100, 100, 255},
        {100, 17, 100, 255},
        {100, 100, 17, 255},
      };
      
      if (ImGui::IsMouseDragging(0))
      {
        if (draggingKeyProps)
        {
          keyPropsPos += io.MouseDelta;
        }
      }

      if (ImGui::IsMouseReleased(0))
      {
        anchorDragged = -1;
        controlDraggedL = -1;
        controlDraggedR = -1;
      }

      ImGui::Text("Curve");
      ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
      ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(85, 85, 85, 200));
      ImGui::BeginChild("Bezier Curve Editor Screen", ImVec2(0, 0), true,
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove);
      ImGui::PopStyleVar(); // WindowPadding
      ImGui::PushItemWidth(120.0f);

      const ImVec2 offset = ImGui::GetCursorScreenPos();
      ImDrawList* draw_list = ImGui::GetWindowDrawList();

      // grid
      {
        ImU32 GRID_COLOR = IM_COL32(17, 17, 17, 51);
        ImU32 CENTRE_COLOR = IM_COL32(17, 17, 17, 153);
        ImVec2 canvas_sz = ImGui::GetWindowSize();
        ImVec2 canvas_bleed = canvas_sz / 10;
        ImVec2 canvas_sz_wbleed = canvas_sz - canvas_bleed * 2; // actual grid size
        // columns
        for (int x = 0; x <= 10; ++x)
        {
          if (x == 0 || x == 5 || x == 10)
            draw_list->AddLine(
              ImVec2(canvas_sz_wbleed.x / 10 * x + canvas_bleed.x, canvas_bleed.y) + offset,
              ImVec2(canvas_sz_wbleed.x / 10 * x + canvas_bleed.x, canvas_sz.y - canvas_bleed.y) + offset,
              CENTRE_COLOR,
              1);
          else
            draw_list->AddLine(
              ImVec2(canvas_sz_wbleed.x / 10 * x + canvas_bleed.x, canvas_bleed.y) + offset,
              ImVec2(canvas_sz_wbleed.x / 10 * x + canvas_bleed.x, canvas_sz.y - canvas_bleed.y) + offset,
              GRID_COLOR,
              1);
          draw_list->AddText(
            ImVec2(canvas_sz_wbleed.x / 10 * x + canvas_bleed.x - 10, canvas_sz.y - canvas_bleed.y + 5) + offset,
            CENTRE_COLOR, (std::to_string(x / 10) + "." + std::to_string(x % 10)).c_str()
          );
        }
        // rows
        for (int y = 0; y <= 10; ++y)
        {
          if (y == 0 || y == 5 || y == 10)
            draw_list->AddLine(
              ImVec2(canvas_bleed.x, canvas_sz_wbleed.y / 10 * y + canvas_bleed.y) + offset,
              ImVec2(canvas_sz.x - canvas_bleed.x, canvas_sz_wbleed.y / 10 * y + canvas_bleed.y) + offset,
              CENTRE_COLOR,
              1);
          else
            draw_list->AddLine(
              ImVec2(canvas_bleed.x, canvas_sz_wbleed.y / 10 * y + canvas_bleed.y) + offset,
              ImVec2(canvas_sz.x - canvas_bleed.x, canvas_sz_wbleed.y / 10 * y + canvas_bleed.y) + offset,
              GRID_COLOR,
              1);
          draw_list->AddText(
            ImVec2(canvas_bleed.x - 30, canvas_sz_wbleed.y / 10 * y + canvas_bleed.y - 10) + offset,
            CENTRE_COLOR, (std::to_string((10 - y) / 10) + "." + std::to_string((10 - y) % 10)).c_str()
          );
        }
        int curveCount = 0;
        for (auto& curve : curves)
        {
          //curve->Sort();
          // actual curve
          size_t first = ~0U;
          size_t last = 0;
          for (size_t i = 0; i < curve->anchors.size() - 1; ++i)
          {
            if (first == ~0U)
              first = i;

            size_t j = 1;

            last = i + j;

            ImVec2 p1 = offset + ImVec2(
              canvas_sz_wbleed.x * curve->anchors[i].x + canvas_bleed.x,
              canvas_sz_wbleed.y * (1 - curve->anchors[i].y) + canvas_bleed.y);
            ImVec2 p2 = offset + ImVec2(
              canvas_sz_wbleed.x * curve->anchors[i + j].x + canvas_bleed.x,
              canvas_sz_wbleed.y * (1 - curve->anchors[i + j].y) + canvas_bleed.y);
            ImVec2 c1 = curve->controlsRight[i].z ? ImVec2(
              canvas_sz_wbleed.x * curve->controlsRight[i].x,
              canvas_sz_wbleed.y * -curve->controlsRight[i].y) : ImVec2();
            ImVec2 c2 = curve->controlsLeft[i + j].z ? ImVec2(
              canvas_sz_wbleed.x * curve->controlsLeft[i + j].x,
              canvas_sz_wbleed.y * -curve->controlsLeft[i + j].y) : ImVec2();

            draw_list->AddBezierCurve(
              p1, p1 + c1,
              p2 + c2, p2,
              cols[curveCount], 1);
          }

          if (/*curve->anchors[first].z &&*/ curve->anchors[first].x > 0)
            draw_list->AddLine(
              ImVec2(canvas_bleed.x,
                canvas_sz_wbleed.y * (1 - curve->anchors[first].y) + canvas_bleed.y) + offset,
              ImVec2(canvas_sz_wbleed.x * curve->anchors[first].x + canvas_bleed.x,
                canvas_sz_wbleed.y * (1 - curve->anchors[first].y) + canvas_bleed.y) + offset,
              IM_COL32(51, 17, 17, 255), 1);

          if (/*curve->anchors[last].z &&*/ curve->anchors[last].x < 1)
            draw_list->AddLine(
              ImVec2(canvas_sz_wbleed.x * curve->anchors[last].x + canvas_bleed.x,
                canvas_sz_wbleed.y * (1 - curve->anchors[last].y) + canvas_bleed.y) + offset,
              ImVec2(canvas_bleed.x + canvas_sz_wbleed.x,
                canvas_sz_wbleed.y * (1 - curve->anchors[last].y) + canvas_bleed.y) + offset,
              IM_COL32(51, 17, 17, 255), 1);

          for (size_t i = 0; i < curve->anchors.size(); ++i)
          {
            ImVec2 p = offset + ImVec2(-3.5f, -3.5f) + ImVec2(
              canvas_sz_wbleed.x * curve->anchors[i].x + canvas_bleed.x,
              canvas_sz_wbleed.y * (1 - curve->anchors[i].y) + canvas_bleed.y);

            ImGui::SetCursorScreenPos(p);
            ImGui::Button(("##BezKey" + std::to_string(i) + " " + std::to_string(curveCount)).c_str(), { 7.f, 7.f });
            if (ImGui::IsItemClicked())
            {
              anchorDragged = static_cast<int>(i);
              selectedKey = static_cast<int>(i);
              selectedCurve = static_cast<int>(curveCount);
            }
          }
          if (selectedCurve == curveCount &&
            anchorDragged >= 0 && anchorDragged < curve->anchors.size())
          {
            curve->anchors[anchorDragged].x += io.MouseDelta.x / canvas_sz_wbleed.x;
            curve->anchors[anchorDragged].y -= io.MouseDelta.y / canvas_sz_wbleed.y;

            curve->anchors[anchorDragged].x = Math::Clamp01(curve->anchors[anchorDragged].x);
            curve->anchors[anchorDragged].y = Math::Clamp01(curve->anchors[anchorDragged].y);
          }
          for (size_t i = 1; i < curve->controlsLeft.size(); ++i)
          {
            if (!curve->controlsLeft[i].z)
              continue;
            ImVec2 p1 = offset + ImVec2(-3.5f, -3.5f) + ImVec2(
              canvas_sz_wbleed.x * curve->anchors[i].x + canvas_bleed.x,
              canvas_sz_wbleed.y * (1 - curve->anchors[i].y) + canvas_bleed.y);
            ImVec2 p2 = p1 + ImVec2(
              canvas_sz_wbleed.x * curve->controlsLeft[i].x,
              canvas_sz_wbleed.y * -curve->controlsLeft[i].y);

            draw_list->AddLine(
              p1 - ImVec2(-3.5f, -3.5f), p2 - ImVec2(-3.5f, -3.5f),
              IM_COL32(100, 100, 100, 255),
              1);

            ImGui::SetCursorScreenPos(p2);
            ImGui::Button(("##BezConL" + std::to_string(i)).c_str(), { 7.f, 7.f });
            if (ImGui::IsItemClicked())
            {
              controlDraggedL = static_cast<int>(i);
            }

            curve->controlsLeft[i].x = Math::Clamp(curve->controlsLeft[i].x, -.1f, .0f);
            curve->controlsLeft[i].y = Math::Clamp(curve->controlsLeft[i].y, -.1f, .1f);
          }
          if (selectedCurve == curveCount &&
            controlDraggedL >= 0 && controlDraggedL < curve->controlsLeft.size())
          {
            curve->controlsLeft[controlDraggedL].x += io.MouseDelta.x / canvas_sz_wbleed.x;
            curve->controlsLeft[controlDraggedL].y -= io.MouseDelta.y / canvas_sz_wbleed.y;
          }
          for (size_t i = 0; i < curve->controlsRight.size() - 1; ++i)
          {
            if (!curve->controlsRight[i].z)
              continue;
            ImVec2 p1 = offset + ImVec2(-3.5f, -3.5f) + ImVec2(
              canvas_sz_wbleed.x * curve->anchors[i].x + canvas_bleed.x,
              canvas_sz_wbleed.y * (1 - curve->anchors[i].y) + canvas_bleed.y);
            ImVec2 p2 = p1 + ImVec2(
              canvas_sz_wbleed.x * curve->controlsRight[i].x,
              canvas_sz_wbleed.y * -curve->controlsRight[i].y);

            draw_list->AddLine(
              p1 - ImVec2(-3.5f, -3.5f), p2 - ImVec2(-3.5f, -3.5f),
              IM_COL32(100, 100, 100, 255),
              1);

            ImGui::SetCursorScreenPos(p2);
            ImGui::Button(("##BezConR" + std::to_string(i)).c_str(), { 7.f, 7.f });
            if (ImGui::IsItemClicked())
            {
              controlDraggedR = static_cast<int>(i);
            }

            curve->controlsRight[i].x = Math::Clamp(curve->controlsRight[i].x, .0f, .1f);
            curve->controlsRight[i].y = Math::Clamp(curve->controlsRight[i].y, -.1f, .1f);
          }
          if (selectedCurve == curveCount &&
            controlDraggedR >= 0 && controlDraggedR < curve->controlsRight.size())
          {
            curve->controlsRight[controlDraggedR].x += io.MouseDelta.x / canvas_sz_wbleed.x;
            curve->controlsRight[controlDraggedR].y -= io.MouseDelta.y / canvas_sz_wbleed.y;
          }
          ImGui::SetCursorScreenPos(offset);
          ImGui::DragFloat("CurveMax", &curve->max, 0.05f);
          ImGui::SetItemAllowOverlap();

          ImGui::SetCursorScreenPos(offset + ImVec2(0, canvas_sz.y - 20));
          ImGui::DragFloat("CurveMin", &curve->min, 0.05f);
          ImGui::SetItemAllowOverlap();

          ++curveCount;
        }

        ImGui::PopItemWidth();
        ImGui::PopStyleColor();
        ImGui::PopStyleVar();

        ImGui::SetItemAllowOverlap();
        if (selectedCurve >= 0 && selectedCurve < curves.size() &&
          selectedKey >= 0 && selectedKey < curves[selectedCurve]->anchors.size())
        {
          ImVec2 p = offset + keyPropsPos;
          ImGui::SetCursorScreenPos(p);
          ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(51, 51, 51, 255));
          if (ImGui::BeginChild("Curve Key Properties", ImVec2(300, 300), true, ImGuiWindowFlags_NoScrollbar))
          {
            static bool denyDrag = true;

            hoveringKeyProps = ImGui::IsWindowHovered();

            ImGui::SetCursorScreenPos(p);
            ImGui::InvisibleButton("##Curve Key Properties Area", ImVec2(300, 300));
            ImGui::SetItemAllowOverlap();
            if (ImGui::IsItemClicked(0))
              denyDrag = false;

            ImGui::SetCursorScreenPos(p);
            ImGui::Text("Key Properties");

            {
              float keyPos[2] = { curves[selectedCurve]->anchors[selectedKey].x, curves[selectedCurve]->anchors[selectedKey].y };
              ImGui::DragFloat2("Pos", keyPos, .001f, 0, 1);
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = true;
              curves[selectedCurve]->anchors[selectedKey].x = keyPos[0];
              curves[selectedCurve]->anchors[selectedKey].y = keyPos[1];
            }
            bool ctrl = curves[selectedCurve]->controlsLeft[selectedKey].z;
            ImGui::Checkbox("Left Control", &ctrl);
            curves[selectedCurve]->controlsLeft[selectedKey].z = ctrl;
            ImGui::SetItemAllowOverlap();
            if (ImGui::IsItemClicked(0))
              denyDrag = true;
            ctrl = curves[selectedCurve]->controlsRight[selectedKey].z;
            ImGui::Checkbox("Right Control", &ctrl);
            curves[selectedCurve]->controlsRight[selectedKey].z = ctrl;
            ImGui::SetItemAllowOverlap();
            if (ImGui::IsItemClicked(0))
              denyDrag = true;

            if (curves[selectedCurve]->controlsLeft[selectedKey].z)
            {
              float conPos[2] = { curves[selectedCurve]->controlsLeft[selectedKey].x, curves[selectedCurve]->controlsLeft[selectedKey].y };
              ImGui::DragFloat2("ConL", conPos, .001f, -.1f, .1f);
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = true;
              curves[selectedCurve]->controlsLeft[selectedKey].x = conPos[0];
              curves[selectedCurve]->controlsLeft[selectedKey].y = conPos[1];
            }
            if (curves[selectedCurve]->controlsRight[selectedKey].z)
            {
              float conPos[2] = { curves[selectedCurve]->controlsRight[selectedKey].x, curves[selectedCurve]->controlsRight[selectedKey].y };
              ImGui::DragFloat2("ConR", conPos, .001f, -.1f, .1f);
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = true;
              curves[selectedCurve]->controlsRight[selectedKey].x = conPos[0];
              curves[selectedCurve]->controlsRight[selectedKey].y = conPos[1];
            }

            draggingKeyProps = !denyDrag;

            if (ImGui::IsMouseReleased(0))
              denyDrag = true;
          }
          ImGui::EndChild();
          ImGui::PopStyleColor();
        }
      }
      ImGui::EndChild();
    }
    ImGui::End();
  }
}