/**********************************************************************************
* \file   AssetPanel.h
* \brief  The file contains interface of AssetPanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "IPanel.h"

namespace DeltaEngine
{
  class AssetPanel : public IPanel
  {
  public:
    AssetPanel(std::string str, Editor& e);
    ~AssetPanel();
    void Render() override;
  };
}
