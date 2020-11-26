#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class ViewportPanel : public IPanel
  {
  public:
    ViewportPanel(std::string str);
    ~ViewportPanel();
    void Render() override;
  private:
    bool m_first_time_viewport = true;
  };
}
