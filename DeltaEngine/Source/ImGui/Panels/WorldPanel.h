/**********************************************************************************
* \file   WorldPanel.h
* \brief  The file contains interface of WorldPanel.
* \author Chin, Clara,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class WorldPanel : public IPanel
  {
  public:
    WorldPanel(std::string str, Editor& e);
    ~WorldPanel();
    void Render() override;
  };
}
