#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class AssetPanel : public IPanel
  {
  public:
    AssetPanel(std::string str, Editor& e);
    ~AssetPanel();
    void Render() override;
  };
}
