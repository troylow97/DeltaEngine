#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class StylePanel : public IPanel
  {
  public:
    StylePanel(std::string str, Editor& e);
    ~StylePanel();
    void Render() override;
  };
}
