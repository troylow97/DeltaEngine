#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class ViewportPanel : public IPanel
  {
  public:
    ViewportPanel(std::string str);
    ~ViewportPanel();
    bool DraggedFileIn();
    void Render(bool) override;
  private:
      bool m_first_time_viewport = true;
  };
}
