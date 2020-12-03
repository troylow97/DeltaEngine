#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"
#include "Render/Texture.h"

namespace DeltaEngine
{
  class SpriteEditorPanel : public IPanel
  {
  public:
    SpriteEditorPanel(std::string str, Editor& e);
    ~SpriteEditorPanel();
    void Enable() override;
    void Render() override;

  private:
    std::vector<TextureInfo> info;
  };
}
