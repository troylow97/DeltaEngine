/**********************************************************************************
* \file   AudioPanel.h
* \brief  The file contains interface of AudioPanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "ImGui/Panels/IPanel.h"
#include "Audio/AudioConfig.h"

namespace DeltaEngine
{
  class AudioPanel : public IPanel
  {

  public:

    AudioPanel(std::string str, Editor& e);
    ~AudioPanel();
    void Render() override;
    static inline AudioConfig config;

  };
}
