/**********************************************************************************
* \file   ButtonsPanel.h
* \brief  The file contains interface of ButtonsPanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class ButtonsPanel : public IPanel
  {
  public:
    ButtonsPanel(std::string str, Editor& e);
    ~ButtonsPanel();
    void Render() override;
  };
}
