/**********************************************************************************
* \file   SettingsPanel.cpp
* \brief  The file contains interface of SettingsPanel. Directly edits the editor
* camera.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class SettingsPanel : public IPanel
  {
  public:
    SettingsPanel(std::string str, Editor& e);
    ~SettingsPanel();
    void Render() override;
  };
}