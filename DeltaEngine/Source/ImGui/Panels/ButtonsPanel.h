#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class ButtonsPanel : public IPanel
  {
  public:
    ButtonsPanel(std::string str, Editor& e);
    ~ButtonsPanel();
    void Render() override;
  };
}
