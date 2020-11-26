#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class SpriteEditorPanel : public IPanel
  {
  public:
    SpriteEditorPanel(std::string str);
    ~SpriteEditorPanel();
    void Render() override;

  private:
    char textureName[128] = "Textures/3-Punch Spritesheet";
  };
}
