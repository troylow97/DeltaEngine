#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class AnimationPanel : public IPanel
  {
  public:
    AnimationPanel(std::string str);
    ~AnimationPanel();
    void Render() override;

  private:
    char clipName[128] = "Clip/3-PunchSpritesheet";

  };
}
