#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class AnimatorPanel : public IPanel
  {
  public:
    AnimatorPanel(std::string str);
    ~AnimatorPanel();
    void Render() override;

  private:
    char animName[128] = "Animation/Dave";

  };
}
