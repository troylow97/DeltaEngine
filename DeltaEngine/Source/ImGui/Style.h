#pragma once
#include "imgui.h"

namespace DeltaEngine
{
void WindowStyle()
{
  auto &style = ImGui::GetStyle();

  // Main Style
  style.WindowPadding = { 8.f,8.f };
  style.FramePadding = { 3.f,3.f };
  style.ItemSpacing = { 8.f,4.f };
  style.ItemInnerSpacing = { 6.f,4.f };
  style.TouchExtraPadding = { 0.f,0.f };
  style.IndentSpacing = 24.f;
  style.ScrollbarSize = 18.f;
  style.GrabMinSize = 18.f;

  // Borders
  style.WindowBorderSize = 1.f;
  style.ChildBorderSize = 1.f;
  style.PopupBorderSize = 1.f;
  style.FrameBorderSize = 1.f;
  style.TabBorderSize = 1.f;

  // Rounding
  style.WindowRounding = 12.f;
  style.ChildRounding = 12.f;
  style.FrameRounding = 12.f;
  style.PopupRounding = 12.f;
  style.ScrollbarRounding = 12.f;
  style.GrabRounding = 12.f;
  style.LogSliderDeadzone = 4.f;
  style.TabRounding = 8.f;

  // Alignment
  style.WindowTitleAlign = {0.02f, 0.5f};
  style.WindowMenuButtonPosition = ImGuiDir_Left;
  style.ColorButtonPosition = ImGuiDir_Right;
  style.ButtonTextAlign = {0.5f, 0.5f};
  style.SelectableTextAlign = {0.f, 0.f};

  // Safe Area Padding
  style.DisplaySafeAreaPadding = { 3.f,3.f };
}
}

void ThemeStyle1()
{
  ImVec4* colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
  colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
  colors[ImGuiCol_WindowBg]               = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_ChildBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_Border]                 = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_BorderShadow]           = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_FrameBg]                = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_FrameBgActive]          = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_TitleBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_TitleBgActive]          = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
  colors[ImGuiCol_MenuBarBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.12f);
  colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_CheckMark]              = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_SliderGrab]             = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_Button]                 = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_ButtonHovered]          = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_ButtonActive]           = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_Header]                 = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_HeaderHovered]          = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_HeaderActive]           = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_Separator]              = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_SeparatorActive]        = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_ResizeGrip]             = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_Tab]                    = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_TabHovered]             = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_TabActive]              = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_TabUnfocused]           = ImVec4(0.30f, 0.10f, 0.14f, 1.00f);
  colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.61f, 0.19f, 0.28f, 1.00f);
  colors[ImGuiCol_DockingPreview]         = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
  colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
  colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
  colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
  colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
  colors[ImGuiCol_DragDropTarget]         = ImVec4(0.45f, 0.15f, 0.21f, 1.00f);
  colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
  colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
  colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
  colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}