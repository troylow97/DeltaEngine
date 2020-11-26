#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class WorldPanel : public IPanel
  {
  public:
    WorldPanel(std::string str);
    ~WorldPanel();
    void Render() override;
  };
}
