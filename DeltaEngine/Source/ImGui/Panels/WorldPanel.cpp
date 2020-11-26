#include "ImGui/Panels/WorldPanel.h"
#include "Input/InputManager.h"
#include "ImGui/Panels/LoggerPanel.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "ImGui//Panels/LoggerPanel.h"
#include "Render/Camera.h"
#include "ImGui/Editor.h"
namespace DeltaEngine
{
WorldPanel::WorldPanel( std::string str ) :
  IPanel( str )
{
  m_enabled = true;
}

WorldPanel::~WorldPanel()
{
  m_enabled = false;
}

void WorldPanel::Render()
{
  ImGui::Begin( m_name.c_str(), &m_enabled );
  auto &em = env.pECS->GetWorld().GetEntityManager();

  if ( ImGui::CollapsingHeader( "Entities" ,ImGuiTreeNodeFlags_DefaultOpen) )
  {
    for ( size_t e_id = 0; e_id < em.GetEntities().size(); e_id++ )
    {
      if ( em.GetEntities()[e_id].chunk )
      {
        const auto &ref = em.GetComponent<EntityName>( { e_id } );
        std::string str = "Entity " + std::to_string( e_id );
        if ( !ref.name.empty() )
          str += " - " + ref.name;

        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf |
          ImGuiTreeNodeFlags_NoTreePushOnOpen;

        if (Editor::entity_selected && Editor::entity_id == e_id)
          node_flags |= ImGuiTreeNodeFlags_Selected
        ;
        ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( e_id ), node_flags, str.c_str() );
        if ( ImGui::IsItemClicked() )
        {
          Editor::entity_selected = true;
          Editor::entity_id = e_id;

          auto &t = em.GetComponent<Transform>( { e_id } );

          Editor::selection_transform = t;
          Editor::selection_transform.scale = { 0.2f, 0.2f, 0.0f };
        }
        if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
          Camera::editorCamera->transform.position = em.GetComponent<Transform>( { e_id } ).position;
      }
    }
  }
  ImGui::End();
}
}
