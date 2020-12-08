/**********************************************************************************
* \file   TilemapPanel.h
* \brief  The file contains interface of TilemapPanel.
* \author Chin, Clara,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include <map>
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class TilemapPanel : public IPanel
  {
  public:
    TilemapPanel(std::string str, Editor& e);
    ~TilemapPanel();
    void Render() override;
  };
}
