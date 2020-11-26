#pragma once

#include <map>
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class TilemapPanel : public IPanel
  {
  public:
    TilemapPanel(std::string str);
    ~TilemapPanel();
    void Render() override;
  };
}
