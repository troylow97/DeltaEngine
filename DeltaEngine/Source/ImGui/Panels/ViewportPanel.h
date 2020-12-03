#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class ViewportPanel : public IPanel
  {
  public:
    ViewportPanel(std::string str, Editor& e);
    ~ViewportPanel();
    void Render() override;
  };
}
