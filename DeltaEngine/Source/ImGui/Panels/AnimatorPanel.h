#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class AnimatorPanel : public IPanel
  {
  public:
    AnimatorPanel(std::string str, Editor& e);
    ~AnimatorPanel();
    void Render() override;
  };
}
