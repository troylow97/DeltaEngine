#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class ButtonsPanel : public IPanel
  {
  public:
    ButtonsPanel(std::string str);
    ~ButtonsPanel();
    void Render() override;
  };
}
