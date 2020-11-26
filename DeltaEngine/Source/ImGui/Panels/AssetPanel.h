#pragma once

#include <memory>
#include "IPanel.h"

namespace DeltaEngine
{
  class AssetPanel : public IPanel
  {
  public:
    AssetPanel(std::string str);
    ~AssetPanel();
    void Render() override;
  };
}
