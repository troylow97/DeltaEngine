#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class WorldPanel : public IPanel
  {
  public:
    WorldPanel(std::string str, Editor& e);
    ~WorldPanel();
    void Render() override;
  };
}
