#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"
#include "Render/Texture.h"

namespace DeltaEngine
{
  class SpriteEditorPanel : public IPanel
  {
  public:
    SpriteEditorPanel(std::string str);
    ~SpriteEditorPanel();
    void Render() override;

  private:
    std::vector<TextureInfo> info;
    char textureName[128] = "Textures/PROPS";
  };
}
