/**********************************************************************************
* \file   SpriteEditorPanel.h
* \brief  The file contains interface of SpriteEditorPanel.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
    bool loaded;
  };
}
