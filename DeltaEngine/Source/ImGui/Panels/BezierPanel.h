/**********************************************************************************
* \file   BezierPanel.h
* \brief  The file contains interface of BezierPanel.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "DEpch.h"
#include "ImGui/Panels/IPanel.h"
#include "Core/Math/BezierCurve.h"

namespace DeltaEngine
{
  class BezierPanel : public IPanel
  {
    bool loaded = false;
  public:
    BezierPanel(std::string str, Editor& e);
    ~BezierPanel();
    void Enable() override;
    void Render() override;

    std::vector<BezierCurve*> curves;
  };
}
