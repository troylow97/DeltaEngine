#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class SettingsPanel : public IPanel
  {
  public:
    SettingsPanel(std::string str, Editor& e);
    ~SettingsPanel();
    void Render() override;
  };
}