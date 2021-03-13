/**********************************************************************************
* \file   WorldPanel.cpp
* \brief  The file contains implementation of WorldPanel.
* \author Chin, Clara,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "ImGui/Panels/WorldPanel.h"
#include "ImGui/Panels/LoggerPanel.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Render/Camera.h"
#include "ImGui/Editor.h"

namespace DeltaEngine
{

struct WorldEntity
{
  size_t id;
  std::vector<size_t> c_id {};
};

using e_p_pair = std::pair<size_t, size_t>;
size_t selection;


WorldPanel::WorldPanel( std::string str, Editor &e ) :
  IPanel( str, e )
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

  std::vector<e_p_pair> e_p_vec;
  std::vector<WorldEntity> entities;

  for ( size_t id = 0; id < em.GetEntities().size(); ++id )
  {
    if ( em.HasComponent<Parent>( { id } ) )
      if ( auto pid = em.GetComponent<Parent>( { id } ).p_id; pid != u64_max )
      {
        e_p_vec.push_back( { id,pid } );
        continue;
      }
    entities.push_back( { id } );
  }

  for ( auto &[e_id, p_id] : e_p_vec )
    for ( auto &[id, c_id_vec] : entities )
      if ( id == p_id )
      {
        c_id_vec.push_back( e_id );
        break;
      }

  if ( ImGui::CollapsingHeader( "Entities", ImGuiTreeNodeFlags_DefaultOpen ) )
  {
    for ( size_t e_id = 0; e_id < entities.size(); e_id++ )
    {
      if ( em.GetEntities()[entities[e_id].id].chunk )
      {
        const auto &ref = em.GetComponent<EntityName>( { entities[e_id].id } );
        std::string str = "Entity " + std::to_string( entities[e_id].id );
        if ( !ref.name.empty() )
          str += " - " + ref.name;

        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;

        if ( Editor::entity_selected && Editor::entity_id == entities[e_id].id )
          node_flags |= ImGuiTreeNodeFlags_Selected;
        ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( entities[e_id].id ), node_flags, str.c_str() );
        if ( ImGui::IsItemClicked() )
        {
          Editor::entity_selected = true;
          Editor::entity_id = entities[e_id].id;

          auto &t = em.GetComponent<Transform>( { entities[e_id].id } );

          Editor::selection_transform = t;
          Editor::selection_transform.scale = { 0.2f, 0.2f, 0.0f };
        }
        ImGui::TreePop();

        if ( ImGui::BeginDragDropTarget() )
        {
          if ( const ImGuiPayload *payload = ImGui::AcceptDragDropPayload( "EntityParent" ); payload )
          {
            size_t id = *static_cast<size_t *>( payload->Data );
            em.SetParents( entities[e_id].id, id );
          }

          ImGui::EndDragDropTarget();
        }

        ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
          ImGuiDragDropFlags_SourceAllowNullID;
        if ( ImGui::BeginDragDropSource( src_flags ) )
        {
          selection = entities[e_id].id;
          ImGui::SetDragDropPayload( "EntityParent", &selection, sizeof( std::string ) );
          ImGui::Text( str.c_str() );
          ImGui::EndDragDropSource();
        }
        if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
          Camera::editorCameraTransform.position = em.GetComponent<Transform>( { entities[e_id].id } ).position;



      }
      ImGui::Indent( 20.0f );
      for ( auto &id : entities[e_id].c_id )
        if ( em.GetEntities()[id].chunk )
        {
          const auto &ref = em.GetComponent<EntityName>( { id } );
          std::string str = "Entity " + std::to_string( id );
          if ( !ref.name.empty() )
            str += " - " + ref.name;

          ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_Leaf;

          if ( Editor::entity_selected && Editor::entity_id == id )
            node_flags |= ImGuiTreeNodeFlags_Selected;
          ImGui::TreeNodeEx( ( void * )static_cast<intptr_t>( id ), node_flags, str.c_str() );
          if ( ImGui::IsItemClicked() )
          {
            Editor::entity_selected = true;
            Editor::entity_id = id;

            auto &t = em.GetComponent<Transform>( { id } );

            Editor::selection_transform = t;
            Editor::selection_transform.scale = { 0.2f, 0.2f, 0.0f };
          }
          ImGui::TreePop();
          ImGuiDragDropFlags src_flags = ImGuiDragDropFlags_SourceNoDisableHover |
            ImGuiDragDropFlags_SourceAllowNullID;
          if ( ImGui::BeginDragDropSource( src_flags ) )
          {
            selection = id;
            ImGui::SetDragDropPayload( "EntityParent", &selection, sizeof( std::string ) );
            ImGui::Text( str.c_str() );
            ImGui::EndDragDropSource();
          }
          if ( ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked( 0 ) )
            Camera::editorCameraTransform.position = em.GetComponent<Transform>( { entities[e_id].id } ).position;
        }
      ImGui::Unindent( 20.0f );
    }
  }
  ImGui::End();
}
}
