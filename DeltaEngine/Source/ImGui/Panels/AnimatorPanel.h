/**********************************************************************************
* \file   AnimatorPanel.h
* \brief  The file contains interface of AnimatorPanel.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"

namespace DeltaEngine
{
  class AnimatorPanel : public IPanel
  {
  public:
    AnimatorPanel(std::string str, Editor& e);
    ~AnimatorPanel();
    void Render() override;
  };
}
