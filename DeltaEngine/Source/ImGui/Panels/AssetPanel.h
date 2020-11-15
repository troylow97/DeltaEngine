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
    bool DraggedFileIn();
    void Render(bool) override;

  private:
    ImVec2 topLeft;
    ImVec2 bottomRight;
  };
}
