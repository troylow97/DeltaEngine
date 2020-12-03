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
