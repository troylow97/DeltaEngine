/**********************************************************************************
* \file   StylePanel.cpp
* \brief  The file contains implementation of StylePanel.
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ImGui/Panels/StylePanel.h"
#include "ECS/ECSModule.h"
#include "ImGui/Editor.h"
namespace DeltaEngine
{
StylePanel::StylePanel( std::string str, Editor& e ) :
  IPanel( str, e )
{
  m_enabled = true;
}

StylePanel::~StylePanel()
{
  m_enabled = false;
}

void StylePanel::Render()
{
  if (ImGui::Begin( m_name.c_str(), &m_enabled ))
    ImGui::ShowStyleEditor();
  ImGui::End();
}
}
