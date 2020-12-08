/**********************************************************************************
* \file   WorldPanel.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
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
