#include "CustomWidgets.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ImGui/ImGuiMath.h"
#include "ImGui/Panels/BezierPanel.h"
#include "examples/imgui_impl_win32.h"

namespace DeltaEngine
{
  bool BezierEdit(const char* label, BezierRange* bezier)
  {
    ImGui::PushID(label);

    std::vector<const char*> c_ptr_vec
    {
      "Constant",
      "Constant Curve",
      "Random Between Constants",
      "Random Between Curves"
    };
    int current = static_cast<int>(bezier->type);
    ImGui::Combo(("##bezier type" + std::string(label)).c_str(), &current, c_ptr_vec.data(), static_cast<int>(c_ptr_vec.size()));
    bezier->type = static_cast<BezierCurve::Type>(current);

    if (bezier->type == BezierCurve::Type::Constant ||
      bezier->type == BezierCurve::Type::RandomBetweenConstants)
    {
      ImGui::DragFloat((std::string(label) + " bezier const 1").c_str(), &bezier->min.min, 0.01f);
    }
    else if (bezier->type == BezierCurve::Type::ConstantCurve ||
      bezier->type == BezierCurve::Type::RandomBetweenCurves)
    {
      if (ImGui::Button("Edit Curve 1..."))
      {
        dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.clear();

        Editor::inst->m_panels[12]->Enable();
        dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->min);
      }
    }

    if (bezier->type == BezierCurve::Type::RandomBetweenConstants)
    {
      ImGui::DragFloat((std::string(label) + " bezier const 2").c_str(), &bezier->max.min, 0.01f);
    }
    else if (bezier->type == BezierCurve::Type::RandomBetweenCurves)
    {
      if (ImGui::Button("Edit Curve 2..."))
      {
        dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.clear();

        Editor::inst->m_panels[12]->Enable();
        dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->max);
      }
    }
    ImGui::PopID();

    return true;
  }

  bool BezierEdit3(const char* label, BezierRange3* bezier)
  {
    ImGuiContext& g = *GImGui;

    ImGui::PushID(label);

    std::vector<const char*> c_ptr_vec
    {
      "Constant",
      "Constant Curve",
      "Random Between Constants",
      "Random Between Curves"
    };
    int current = static_cast<int>(bezier->type);
    ImGui::Combo(("##bezier3 type" + std::string(label)).c_str(), &current, c_ptr_vec.data(), static_cast<int>(c_ptr_vec.size()));
    bezier->type = static_cast<BezierCurve::Type>(current);

    if (bezier->type == BezierCurve::Type::Constant ||
      bezier->type == BezierCurve::Type::RandomBetweenConstants)
    {
      if (bezier->type == BezierCurve::Type::RandomBetweenConstants)
        ImGui::Text("Min");
      ImGui::PushID((std::string(label) + " bezier3 const 1").c_str());
      float col[3] =
      {
        bezier->minX.min,
        bezier->minY.min,
        bezier->minZ.min
      };

      ImGui::DragFloat3("min", col, 0.01f);

      bezier->minX.min = col[0];
      bezier->minY.min = col[1];
      bezier->minZ.min = col[2];
      ImGui::PopID();

      if (bezier->type == BezierCurve::Type::RandomBetweenConstants)
      {
        ImGui::Text("Max");
        ImGui::PushID((std::string(label) + " bezier3 const 2").c_str());
        float cols[3] =
        {
          bezier->maxX.min,
          bezier->maxY.min,
          bezier->maxZ.min
        };

        ImGui::DragFloat3("max", cols, 0.01f);

        bezier->maxX.min = cols[0];
        bezier->maxY.min = cols[1];
        bezier->maxZ.min = cols[2];
        ImGui::PopID();
      }
    }
    else if (bezier->type == BezierCurve::Type::ConstantCurve ||
      bezier->type == BezierCurve::Type::RandomBetweenCurves)
    {
      if (bezier->type == BezierCurve::Type::RandomBetweenCurves)
        ImGui::Text("Min");

      ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
      for (int i = 0; i < 3; i++)
      {
        ImGui::PushID((std::string(label) + " Min " + std::to_string(i)).c_str());
        if (i > 0)
          ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);

        if (i == 0)
          ImGui::Checkbox("X", &bezier->minXActive);
        if (i == 1)
          ImGui::Checkbox("Y", &bezier->minYActive);
        if (i == 2)
          ImGui::Checkbox("Z", &bezier->minZActive);

        ImGui::PopID();
        ImGui::PopItemWidth();
      }
      if (bezier->type == BezierCurve::Type::RandomBetweenCurves)
      {
        ImGui::Text("Max");

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        for (int i = 0; i < 3; i++)
        {
          ImGui::PushID((std::string(label) + "Max " + std::to_string(i)).c_str());
          if (i > 0)
            ImGui::SameLine(0, g.Style.ItemInnerSpacing.x);

          if (i == 0)
            ImGui::Checkbox("X", &bezier->maxXActive);
          if (i == 1)
            ImGui::Checkbox("Y", &bezier->maxYActive);
          if (i == 2)
            ImGui::Checkbox("Z", &bezier->maxZActive);

          ImGui::PopID();
          ImGui::PopItemWidth();
        }
      }
    }
    if (bezier->type == BezierCurve::Type::ConstantCurve ||
      bezier->type == BezierCurve::Type::RandomBetweenCurves)
    {
      if (ImGui::Button("Edit Curve..."))
      {
        dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.clear();

        Editor::inst->m_panels[12]->Enable();
        if (bezier->minXActive)
          dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->minX);
        if (bezier->maxXActive)
          dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->maxX);
        if (bezier->minYActive)
          dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->minY);
        if (bezier->maxYActive)
          dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->maxY);
        if (bezier->minZActive)
          dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->minZ);
        if (bezier->maxZActive)
          dynamic_cast<BezierPanel*>(Editor::inst->m_panels[12].get())->curves.push_back(&bezier->maxZ);
      }
    }
    ImGui::PopID();

    return true;
  }

  void DrawAlphaMarks(Gradient* gradient,
    int* draggingAlpha,
    int* selectedAlpha,
    struct ImVec2 const& bar_pos,
    float maxWidth,
    float height)
  {
    ImVec4 colorA = { 1,1,1,1 };
    ImVec4 colorB = { 1,1,1,1 };
    float barTop = bar_pos.y;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 colorAU32 = 0;
    ImU32 colorBU32 = 0;

    float previousLocation = -1;
    Color previousColor = Color();
    int count = 0;

    for (auto& [value, location] : gradient->alphaKeys)
    {
      float to = bar_pos.x + location * maxWidth;

      if (previousLocation < 0)
      {
        colorA.x = value;
        colorA.y = value;
        colorA.z = value;
      }
      else
      {
        colorA.x = previousColor.r;
        colorA.y = previousColor.g;
        colorA.z = previousColor.b;
      }

      colorB.x = value;
      colorB.y = value;
      colorB.z = value;

      colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
      colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

      draw_list->AddTriangleFilled(
        ImVec2(to, barTop + 18),
        ImVec2(to - 6, barTop + 12),
        ImVec2(to + 6, barTop + 12), IM_COL32(100, 100, 100, 255));

      draw_list->AddRectFilled(
        ImVec2(to - 6, barTop),
        ImVec2(to + 6, barTop + 12),
        IM_COL32(100, 100, 100, 255), 1.0f, 1);

      draw_list->AddRectFilled(
        ImVec2(to - 5, barTop + 1),
        ImVec2(to + 5, barTop + 11),
        IM_COL32(0, 0, 0, 255), 1.0f, 1);

      if (*selectedAlpha == count)
      {
        draw_list->AddTriangleFilled(
          ImVec2(to, barTop + 17),
          ImVec2(to - 4, barTop + 13),
          ImVec2(to + 4, barTop + 13), IM_COL32(0, 255, 0, 255));

        draw_list->AddRect(
          ImVec2(to - 5, barTop + 1),
          ImVec2(to + 5, barTop + 11),
          IM_COL32(0, 255, 0, 255), 1.0f, 1);
      }

      draw_list->AddRectFilled(
        ImVec2(to - 3, barTop + 3),
        ImVec2(to + 3, barTop + 9),
        IM_COL32(value * 255, value * 255, value * 255, 255), 1.0f, 1);

      ImGui::SetCursorScreenPos(ImVec2(to - 6, barTop));
      ImGui::InvisibleButton("mark", ImVec2(12, 18));

      if (ImGui::IsItemHovered())
      {
        if (ImGui::IsMouseClicked(0))
        {
          *selectedAlpha = count;
          *draggingAlpha = count;
        }
      }

      previousLocation = location;
      previousColor = Color(value, value, value);
      ++count;
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 20.0f));
  }

  void DrawGradientMarks(Gradient* gradient,
    int* draggingColor,
    int* selectedColor,
    struct ImVec2 const& bar_pos,
    float maxWidth,
    float height)
  {
    ImVec4 colorA = { 1,1,1,1 };
    ImVec4 colorB = { 1,1,1,1 };
    float barBottom = bar_pos.y + height;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImU32 colorAU32 = 0;
    ImU32 colorBU32 = 0;

    float previousLocation = -1;
    Color previousColor = Color();
    int count = 0;

    for (auto& color : gradient->colorKeys)
    {
      float to = bar_pos.x + color.a * maxWidth;

      if (previousLocation < 0)
      {
        colorA.x = color.r;
        colorA.y = color.g;
        colorA.z = color.b;
      }
      else
      {
        colorA.x = previousColor.r;
        colorA.y = previousColor.g;
        colorA.z = previousColor.b;
      }

      colorB.x = color.r;
      colorB.y = color.g;
      colorB.z = color.b;

      colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
      colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

      draw_list->AddTriangleFilled(
        ImVec2(to, barBottom),
        ImVec2(to - 6, barBottom + 6),
        ImVec2(to + 6, barBottom + 6), IM_COL32(100, 100, 100, 255));

      draw_list->AddRectFilled(
        ImVec2(to - 6, barBottom + 6),
        ImVec2(to + 6, barBottom + 18),
        IM_COL32(100, 100, 100, 255), 1.0f, 1);

      draw_list->AddRectFilled(
        ImVec2(to - 5, barBottom + 7),
        ImVec2(to + 5, barBottom + 17),
        IM_COL32(0, 0, 0, 255), 1.0f, 1);

      if (*selectedColor == count)
      {
        draw_list->AddTriangleFilled(
          ImVec2(to, barBottom + 1),
          ImVec2(to - 4, barBottom + 5),
          ImVec2(to + 4, barBottom + 5), IM_COL32(0, 255, 0, 255));

        draw_list->AddRect(
          ImVec2(to - 5, barBottom + 7),
          ImVec2(to + 5, barBottom + 17),
          IM_COL32(0, 255, 0, 255), 1.0f, 1);
      }

      draw_list->AddRectFilledMultiColor(
        ImVec2(to - 3, barBottom + 9),
        ImVec2(to + 3, barBottom + 15),
        colorBU32, colorBU32, colorBU32, colorBU32);

      ImGui::SetCursorScreenPos(ImVec2(to - 6, barBottom));
      ImGui::InvisibleButton("mark", ImVec2(12, 18));

      if (ImGui::IsItemHovered())
      {
        if (ImGui::IsMouseClicked(0))
        {
          *selectedColor = count;
          *draggingColor = count;
        }
      }

      previousLocation = color.a;
      previousColor = color;
      ++count;
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 20.0f));
  }

  void DrawGradientBar(Gradient* gradient,
    ImVec2 const& bar_pos,
    float maxWidth,
    float height)
  {
    ImVec4 colorA = { 1,1,1,1 };
    ImVec4 colorB = { 1,1,1,1 };
    float prevX = bar_pos.x;
    float barBottom = bar_pos.y + height;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    draw_list->AddRectFilled(ImVec2(bar_pos.x - 2, bar_pos.y - 2),
      ImVec2(bar_pos.x + maxWidth + 2, barBottom + 2),
      IM_COL32(100, 100, 100, 255));

    if (gradient->colorKeys.size() == 0)
    {
      draw_list->AddRectFilled(ImVec2(bar_pos.x, bar_pos.y),
        ImVec2(bar_pos.x + maxWidth, barBottom),
        IM_COL32(255, 255, 255, 255));
    }

    ImU32 colorAU32 = 0;
    ImU32 colorBU32 = 0;

    float previousLocation = -1;
    Color previousColor = Color();

    for (auto& color : gradient->colorKeys)
    {
      if (color.a < 0)
      {
        continue;
      }

      float from = prevX;
      float to = prevX = bar_pos.x + color.a * maxWidth;

      if (previousLocation < 0)
      {
        colorA.x = color.r;
        colorA.y = color.g;
        colorA.z = color.b;
      }
      else
      {
        colorA.x = previousColor.r;
        colorA.y = previousColor.g;
        colorA.z = previousColor.b;
      }

      colorB.x = color.r;
      colorB.y = color.g;
      colorB.z = color.b;

      colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
      colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

      if (color.a >= 0.0)
      {
        draw_list->AddRectFilledMultiColor(ImVec2(from, bar_pos.y),
          ImVec2(to, barBottom),
          colorAU32, colorBU32, colorBU32, colorAU32);
      }

      previousLocation = color.a;
      previousColor = color;
    }

    if (previousLocation < 1.0)
    {
      draw_list->AddRectFilledMultiColor(ImVec2(prevX, bar_pos.y),
        ImVec2(bar_pos.x + maxWidth, barBottom),
        colorBU32, colorBU32, colorBU32, colorBU32);
    }

    ImGui::SetCursorScreenPos(ImVec2(bar_pos.x, bar_pos.y + height + 10.0f));
  }

  bool GradientEdit(const char* label, GradientRange* gradient)
  {
    std::vector<const char*> c_ptr_vec
    {
      "Constant Color",
      "Constant Gradient",
      "Random Between Colors",
      "Random Between Gradients"
    };
    int current = static_cast<int>(gradient->type);
    ImGui::Combo(("##gradient type" + std::string(label)).c_str(), &current, c_ptr_vec.data(), static_cast<int>(c_ptr_vec.size()));
    gradient->type = static_cast<Gradient::Type>(current);

    if (gradient->type == Gradient::Type::ConstantColor ||
      gradient->type == Gradient::Type::RandomBetweenColors)
    {
      float col[4] =
      {
        gradient->min.colorKeys[0].r,
        gradient->min.colorKeys[0].g,
        gradient->min.colorKeys[0].b,
        gradient->min.alphaKeys[0].x
      };
      ImGui::ColorEdit4((std::string(label) + " color 1").c_str(), col);
      gradient->min.colorKeys[0].r = col[0];
      gradient->min.colorKeys[0].g = col[1];
      gradient->min.colorKeys[0].b = col[2];
      gradient->min.alphaKeys[0].x = col[3];
    }
    else if (gradient->type == Gradient::Type::ConstantGradient ||
      gradient->type == Gradient::Type::RandomBetweenGradients)
    {
      float maxWidth = ImMax(250.0f, ImGui::GetContentRegionAvailWidth() - 100.0f);
      ImVec2 bar_pos = ImGui::GetCursorScreenPos();
      static int draggingAlpha = 0;
      static int selectedAlpha = 0;
      static int draggingColor = 0;
      static int selectedColor = 0;

      DrawGradientBar(&gradient->min, bar_pos + ImVec2(0, 18), maxWidth, 25);
      DrawAlphaMarks(&gradient->min, &draggingAlpha, &selectedAlpha, bar_pos, maxWidth, 25);
      DrawGradientMarks(&gradient->min, &draggingColor, &selectedColor, bar_pos + ImVec2(0, 18), maxWidth, 25);

      if (selectedColor >= 0 && selectedColor < gradient->min.colorKeys.size())
      {
        float col[3] =
        {
          gradient->min.colorKeys[selectedColor].r,
          gradient->min.colorKeys[selectedColor].g,
          gradient->min.colorKeys[selectedColor].b
        };
        ImGui::TextEx("Color");
        ImGui::ColorEdit3((std::string(label) + " gradient 1 color " + std::to_string(selectedColor)).c_str(), col);
        gradient->min.colorKeys[selectedColor].r = col[0];
        gradient->min.colorKeys[selectedColor].g = col[1];
        gradient->min.colorKeys[selectedColor].b = col[2];
        ImGui::TextEx("Color Location");
        ImGui::DragFloat((std::string(label) + " color 1 location " + std::to_string(selectedColor)).c_str(),
          &gradient->min.colorKeys[selectedColor].a, 0.01f, 0, 1);
      }

      if (selectedAlpha >= 0 && selectedAlpha < gradient->min.colorKeys.size())
      {
        ImGui::TextEx("Alpha");
        ImGui::DragFloat((std::string(label) + " alpha 1 " + std::to_string(selectedAlpha)).c_str(),
          &gradient->min.alphaKeys[selectedAlpha].x, 0.01f, 0, 1);
        ImGui::TextEx("Alpha Location");
        ImGui::DragFloat((std::string(label) + " alpha 1 location " + std::to_string(selectedAlpha)).c_str(),
          &gradient->min.alphaKeys[selectedAlpha].y, 0.01f, 0, 1);
      }

      if (!ImGui::IsMouseDown(0) && draggingColor >= 0)
      {
        draggingColor = -1;
      }

      if (ImGui::IsMouseDragging(0) && draggingColor >= 0)
      {
        float increment = ImGui::GetIO().MouseDelta.x / maxWidth;
        bool insideZone = (ImGui::GetIO().MousePos.x > bar_pos.x) &&
          (ImGui::GetIO().MousePos.x < bar_pos.x + maxWidth);

        if (increment != 0.0f && insideZone)
        {
          gradient->min.colorKeys[draggingColor].a += increment;
          gradient->min.colorKeys[draggingColor].a = Math::Clamp01(gradient->min.colorKeys[draggingColor].a);
        }
      }

      if (!ImGui::IsMouseDown(0) && draggingAlpha >= 0)
      {
        draggingAlpha = -1;
      }

      if (ImGui::IsMouseDragging(0) && draggingAlpha >= 0)
      {
        float increment = ImGui::GetIO().MouseDelta.x / maxWidth;
        bool insideZone = (ImGui::GetIO().MousePos.x > bar_pos.x) &&
          (ImGui::GetIO().MousePos.x < bar_pos.x + maxWidth);

        if (increment != 0.0f && insideZone)
        {
          gradient->min.alphaKeys[draggingAlpha].y += increment;
          gradient->min.alphaKeys[draggingAlpha].y = Math::Clamp01(gradient->min.alphaKeys[draggingAlpha].y);
        }
      }
    }

    if (gradient->type == Gradient::Type::RandomBetweenColors)
    {
      float col[4] =
      {
        gradient->max.colorKeys[0].r,
        gradient->max.colorKeys[0].g,
        gradient->max.colorKeys[0].b,
        gradient->max.alphaKeys[0].x
      };
      ImGui::ColorEdit4((std::string(label) + " color 2").c_str(), col);
      gradient->max.colorKeys[0].r = col[0];
      gradient->max.colorKeys[0].g = col[1];
      gradient->max.colorKeys[0].b = col[2];
      gradient->max.alphaKeys[0].x = col[3];
    }
    else if (gradient->type == Gradient::Type::RandomBetweenGradients)
    {
      float maxWidth = ImMax(250.0f, ImGui::GetContentRegionAvailWidth() - 100.0f);
      ImVec2 bar_pos = ImGui::GetCursorScreenPos();
      static int draggingAlpha = 0;
      static int selectedAlpha = 0;
      static int draggingColor = 0;
      static int selectedColor = 0;

      DrawGradientBar(&gradient->max, bar_pos + ImVec2(0, 18), maxWidth, 25);
      DrawAlphaMarks(&gradient->max, &draggingAlpha, &selectedAlpha, bar_pos, maxWidth, 25);
      DrawGradientMarks(&gradient->max, &draggingColor, &selectedColor, bar_pos + ImVec2(0, 18), maxWidth, 25);

      if (selectedColor >= 0 && selectedColor < gradient->max.colorKeys.size())
      {
        float col[3] =
        {
          gradient->max.colorKeys[selectedColor].r,
          gradient->max.colorKeys[selectedColor].g,
          gradient->max.colorKeys[selectedColor].b
        };
        ImGui::TextEx("Color");
        ImGui::ColorEdit3((std::string(label) + " gradient 2 color " + std::to_string(selectedColor)).c_str(), col);
        gradient->max.colorKeys[selectedColor].r = col[0];
        gradient->max.colorKeys[selectedColor].g = col[1];
        gradient->max.colorKeys[selectedColor].b = col[2];
        ImGui::TextEx("Color Location");
        ImGui::DragFloat((std::string(label) + " color 2 location " + std::to_string(selectedColor)).c_str(),
          &gradient->max.colorKeys[selectedColor].a, 0.01f, 0, 1);
      }

      if (selectedAlpha >= 0 && selectedAlpha < gradient->max.colorKeys.size())
      {
        ImGui::TextEx("Alpha");
        ImGui::DragFloat((std::string(label) + " alpha 2 " + std::to_string(selectedAlpha)).c_str(),
          &gradient->max.alphaKeys[selectedAlpha].x, 0.01f, 0, 1);
        ImGui::TextEx("Alpha Location");
        ImGui::DragFloat((std::string(label) + " alpha 2 location " + std::to_string(selectedAlpha)).c_str(),
          &gradient->max.alphaKeys[selectedAlpha].y, 0.01f, 0, 1);
      }

      if (!ImGui::IsMouseDown(0) && draggingColor >= 0)
      {
        draggingColor = -1;
      }

      if (ImGui::IsMouseDragging(0) && draggingColor >= 0)
      {
        float increment = ImGui::GetIO().MouseDelta.x / maxWidth;
        bool insideZone = (ImGui::GetIO().MousePos.x > bar_pos.x) &&
          (ImGui::GetIO().MousePos.x < bar_pos.x + maxWidth);

        if (increment != 0.0f && insideZone)
        {
          gradient->max.colorKeys[draggingColor].a += increment;
          gradient->max.colorKeys[draggingColor].a = Math::Clamp01(gradient->max.colorKeys[draggingColor].a);
        }
      }

      if (!ImGui::IsMouseDown(0) && draggingAlpha >= 0)
      {
        draggingAlpha = -1;
      }

      if (ImGui::IsMouseDragging(0) && draggingAlpha >= 0)
      {
        float increment = ImGui::GetIO().MouseDelta.x / maxWidth;
        bool insideZone = (ImGui::GetIO().MousePos.x > bar_pos.x) &&
          (ImGui::GetIO().MousePos.x < bar_pos.x + maxWidth);

        if (increment != 0.0f && insideZone)
        {
          gradient->max.alphaKeys[draggingAlpha].y += increment;
          gradient->max.alphaKeys[draggingAlpha].y = Math::Clamp01(gradient->max.alphaKeys[draggingAlpha].y);
        }
      }
    }
    return true;
  }
}
