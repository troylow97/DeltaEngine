#pragma once

#include <map>
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class TilemapPanel : public IPanel
  {
  public:
    TilemapPanel(std::string str, Editor& e);
    ~TilemapPanel();
    void Render() override;
  };
}
