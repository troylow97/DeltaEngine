#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  enum ImGuiIconEnum
  {
    ImGui_fa_play = 0xF04B,
    ImGui_fa_pause = 0xF04C
  };

  class ButtonsPanel : public IPanel
  {
  public:
    ButtonsPanel(std::string str);
    ~ButtonsPanel();
    bool DraggedFileIn();
    void Render(bool) override;

  private:
    ImVec2 topLeft;
    ImVec2 bottomRight;
  };
}
