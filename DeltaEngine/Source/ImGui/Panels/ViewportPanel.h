#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class ViewportPanel : public IPanel
  {
  public:
    ViewportPanel(std::string str, Editor& e);
    ~ViewportPanel();
    static inline Vector2 ren_pos{0,0};
    static inline Vector2 ren_size{0,0};
    void Render() override;
  };
}
