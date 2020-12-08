/**********************************************************************************
* \file   PropertyInspectorPanel.h
* \brief  The file contains interface of PropertyInspectorPanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "ImGui/Panels/IPanel.h"

namespace rttr
{
  class instance;
}

namespace DeltaEngine
{
  class PropertyInspectorPanel : public IPanel
  {
  public:
    PropertyInspectorPanel(std::string str, Editor& e);
    ~PropertyInspectorPanel();
    void Render() override;
  };
}
