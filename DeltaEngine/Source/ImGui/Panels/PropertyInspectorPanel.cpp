#include "ImGui/Panels/PropertyInspectorPanel.h"
#include "ImGui/IconsFontAwesome5.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "Render/Texture.h"

#include "Assets/AssetManager.h"
#include "Reflect/Reflect.h"
#include <filesystem>

#include "ImGui/Editor.h"

namespace DeltaEngine
{
PropertyInspectorPanel::PropertyInspectorPanel( std::string str, Editor &e ) :
  IPanel( str, e )
{
  m_enabled = true;
}

PropertyInspectorPanel::~PropertyInspectorPanel()
{
  m_enabled = false;
}

void PropertyInspectorPanel::Render()
{
  ImGui::Begin( m_name.c_str(), &m_enabled, ImGuiWindowFlags_NoScrollbar );
  if ( Editor::entity_selected )
  {
    auto &em = env.pECS->GetWorld().GetEntityManager();

    size_t index = Editor::entity_id;
    if ( em.HasComponent<EntityName>( { index } ) )
    {
      const auto &entity = em.GetComponent<EntityName>( { index } );
      std::string text {};
      if ( entity.name.empty() )
        text = "Entity " + std::to_string( index ) + "'s Properties";
      else
        text = entity.name + "'s Properties";

      ImGui::Text( text.c_str() );
      ImGui::Separator();
      ImGui::Text( "" );
    }

    if ( auto result = em.GetEntityArchetype( index ); result != nullptr )
    {
      std::vector<std::string> c_list;
      c_list.push_back( " " );
      for ( auto &ref : ComponentMeta::GetComponentMetaArray() )
        if ( !( ref.bits & result->bits_signature ) && ref.bits != 1 )
          c_list.push_back( RT_Reflect::RT_Checker( ref.bits ).get_name().to_string() );

      static size_t selected = 0;

      ImGui::PushItemWidth( ImGui::GetWindowWidth() - 55.0f );

      const char *label = c_list[selected].c_str();
      ImGui::Text( "Add Components" );
      if ( ImGui::BeginCombo( "##AddComponents", label ) )
      {
        for ( size_t i = 0; i < c_list.size(); i++ )
        {
          const bool is_selected = ( selected == i );
          if ( ImGui::Selectable( c_list[i].c_str(), is_selected ) )
            selected = i;

          if ( is_selected )
            ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
      }
      ImGui::SameLine();
      const float original = Editor::font_awesome->Scale;
      Editor::font_awesome->Scale = 0.5f;
      ImGui::PushFont( Editor::font_awesome );
      if ( ImGui::Button( ICON_FA_PLUS_CIRCLE, { 30.0f,26.0f } ) )
        if ( selected )
        {
          RT_Reflect::RT_Setter( em, { index },
                                 rttr::type::get_by_name( c_list[selected] ).get_metadata( "bits" ).to_uint64() );
          selected = 0;
          ImGui::PopFont();
          Editor::font_awesome->Scale = original;
          ImGui::End();
          return;
        }
      ImGui::PopFont();
      Editor::font_awesome->Scale = original;

      ImGui::Text( "" );
      ImGui::PopItemWidth();
      ImGui::Separator();

      for ( auto &ref : result->components_desc->metalist )
      {
        rttr::instance instance = em.GetComponent( { Editor::entity_id }, ref.meta->bits );

        ImGui::BeginGroup();
        if ( ImGui::CollapsingHeader( instance.get_type().get_name().to_string().c_str(), ImGuiTreeNodeFlags_AllowItemOverlap ) )
        {
          ImGui::PushItemWidth( ImGui::GetWindowWidth() - 20.0f );
          ImGui::Dummy( { 0.0f, 8.0f } );

          bool pad { false };

          for ( auto property : instance.get_type().get_properties() )
          {
            rttr::variant value = property.get_value( instance );
            if ( !value || property.get_metadata( "NO_EDITOR" ) )
              continue;

            auto prop_type = property.get_type();
            auto prop_name = property.get_name().to_string();

            ImGui::Text( prop_name.c_str() );

            if ( prop_name == "Size" )
              prop_name.assign( "Size" + instance.get_type().get_name().to_string() );

            if ( prop_type == rttr::type::get<float *>() )
              ImGui::DragFloat( ( "##" + prop_name ).c_str(), ( value.get_value<float *>() ), 0.01f );
            else if ( prop_type == rttr::type::get<int *>() )
              ImGui::InputInt( ( "##" + prop_name ).c_str(), ( value.get_value<int *>() ), 1 );
            else if ( prop_type == rttr::type::get<unsigned *>() )
              ImGui::InputInt( ( "##" + prop_name ).c_str(), (int *) ( value.get_value<unsigned *>() ), 1, 0 );
            else if ( prop_type == rttr::type::get<Vector2 *>() )
              ImGui::DragFloat2( ( "##" + prop_name ).c_str(), (float *) ( value.get_value<Vector2 *>() ), 0.01f );
            else if ( prop_type == rttr::type::get<Vector3 *>() )
              ImGui::DragFloat3( ( "##" + prop_name ).c_str(), (float *) ( value.get_value<Vector3 *>() ), 0.01f );
            else if ( prop_type == rttr::type::get<Quaternion *>() )
              ImGui::DragFloat4( ( "##" + prop_name ).c_str(), (float *) ( value.get_value<Quaternion *>() ), 0.01f );
            else if ( prop_type == rttr::type::get<bool *>() )
              ImGui::Checkbox( ( "##" + prop_name ).c_str(), value.get_value<bool *>() );
            else if ( prop_type == rttr::type::get<std::string *>() && (
              instance.get_type() == rttr::type::get<AI>() ||
              instance.get_type() == rttr::type::get<EntityName>() ||
              instance.get_type() == rttr::type::get<Text>() ||
              instance.get_type() == rttr::type::get<UI>() ) )
            {
              auto &str = *value.get_value<std::string *>();
              char buffer[256] {};
              if ( ImGui::InputText( ( "##" + prop_name ).c_str(), buffer, sizeof( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
                str = std::string( buffer );
            }
            else if ( prop_type.get_raw_type().is_enumeration() )
            {
              rttr::enumeration enum_prop = prop_type.get_raw_type().get_enumeration();
              auto enum_list = enum_prop.get_names();
              std::vector<const char *> c_ptr_vec;
              for ( auto &e_name : enum_list )
                c_ptr_vec.push_back( e_name.data() );
              int current = value.get_wrapped_value<unsigned>();
              ImGui::Combo( ( "##" + prop_name ).c_str(), &current, c_ptr_vec.data(), static_cast<int>( c_ptr_vec.size() ) );
              const_cast<unsigned &>( value.get_wrapped_value<unsigned>() ) = current;
            }
            else if ( prop_type == rttr::type::get<Color *>() )
            {
              auto &color = *value.get_value<Color *>();
              ImGui::ColorPicker4( ( "##" + prop_name ).c_str(), (float *) &color, ImGuiColorEditFlags_NoSidePreview );
            }
            else if ( prop_type == rttr::type::get<Sprite *>() )
            {
              auto &sprite = *value.get_value<Sprite *>();

              auto tex = GetEnv().pManager->Get<Texture2D>( sprite.m_Key );
              bool error { false };
              if ( tex.State() == AssetState::NotFound || 
                   tex.State() == AssetState::NotFoundFallback || 
                   tex.State() == AssetState::NotLoaded || 
                   tex.State() == AssetState::NotLoadedFallback )
              {
                error = true;
                ImGui::Text( "Error Sprite Key - %s", sprite.m_Key.c_str() );
              }


              std::vector<std::string> tex_key_vec;
              tex_key_vec.push_back( " " );
              for ( auto &[key, data] : GetEnv().pManager->List<Texture2D>() )
                if ( data )
                  for ( size_t i = 0; i < data->textureInfo.size(); i++ )
                    tex_key_vec.push_back( key.Key() + '_' + std::to_string( i ) );

              size_t selection = 0;
              for ( size_t i = 0; i < tex_key_vec.size(); i++ )
                if ( ( sprite.m_Key + '_' + std::to_string( sprite.m_Index ) ) == tex_key_vec[i] )
                  selection = i;


              size_t initial = selection;
              const char *tex_key = tex_key_vec[selection].c_str();
              ImGui::PushItemWidth( ImGui::GetWindowWidth() - 55.0f );
              if ( ImGui::BeginCombo( ( "##" + prop_name ).c_str(), tex_key ) )
              {
                for ( size_t i = 0; i < tex_key_vec.size(); i++ )
                {
                  const bool is_selected = ( selection == i );
                  if ( ImGui::Selectable( tex_key_vec[i].c_str(), is_selected ) )
                    selection = i;

                  if ( is_selected )
                    ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
              ImGui::PopItemWidth();

              if ( initial != selection )
              {
                auto offset = tex_key_vec[selection].find_last_of( '_' );
                sprite.m_Key = tex_key_vec[selection].substr( 0, offset );
                sprite.m_Index = std::stoi( tex_key_vec[selection].substr( offset + 1 ) );
              }
              if ( ImGui::BeginDragDropTarget() )
              {
                const ImGuiPayload *assetpayload = ImGui::AcceptDragDropPayload( "ASSETFILES" );
                if ( assetpayload )
                {
                  std::string assetpayload_n = *static_cast<std::string *>( assetpayload->Data );
                  std::filesystem::path file { assetpayload_n };
                  if ( file.extension() == ".png" || file.extension() == ".jpg" )
                  {
                    sprite.m_Key = assetpayload_n.substr( 0, assetpayload_n.find_last_of( '.' ) );
                    sprite.m_Index = 0;
                  }
                }
                ImGui::EndDragDropTarget();
              }
              ImGui::SameLine();
              Editor::font_awesome->Scale = 0.5f;
              ImGui::PushFont( Editor::font_awesome );
              static bool clicked = false;
              if ( ImGui::Button( ICON_FA_DOT_CIRCLE, { 26.0f, 26.0f } ) )
                clicked = !clicked;
              ImGui::PopFont();
              Editor::font_awesome->Scale = original;

              // Sprite Panel
              if ( clicked )
              {
                ImGui::Begin( "Sprite preview", &clicked, ImGuiWindowFlags_AlwaysAutoResize );
                for ( size_t i = 0; i < sprite.GetTexture()->textureInfo.size() - 1; i++ )
                {
                  if ( i % 4 )
                    ImGui::SameLine();
                  Sprite details { sprite.m_Key, static_cast<unsigned>( i ) };
                  ImGui::Image( reinterpret_cast<ImTextureID>( static_cast<size_t>( details.GetTexture()->GetRendererID() ) ),
                                ImVec2 { 64, 64 },
                                ImVec2 { details.GetOffset().x, details.GetOffset().y },
                                ImVec2 {
                                  details.GetOffset().x + details.GetTiling().x,
                                  details.GetOffset().y + details.GetTiling().y
                                } );
                }
                ImGui::End();
              }
            }
            else if ( prop_type == rttr::type::get<std::string *>() && instance.get_type() == rttr::type::get<Animator>() )
            {
              std::vector<std::string> anim_key_vec;
              anim_key_vec.push_back( " " );

              for ( const auto &[key, data] : GetEnv().pManager->List<AnimationController>() )
                anim_key_vec.push_back( key.Key() );

              auto &str = *value.get_value<std::string *>();
              size_t selection = 0;

              for ( size_t i = 0; i < anim_key_vec.size(); i++ )
                if ( str == anim_key_vec[i] )
                  selection = i;

              size_t initial = selection;
              const char *anim_key = anim_key_vec[selection].c_str();

              if ( ImGui::BeginCombo( ( "##" + prop_name ).c_str(), anim_key ) )
              {
                for ( size_t i = 0; i < anim_key_vec.size(); i++ )
                {
                  const bool is_selected = ( selection == i );
                  if ( ImGui::Selectable( anim_key_vec[i].c_str(), is_selected ) )
                    selection = i;

                  if ( is_selected )
                    ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
              if ( initial != selection )
                str = anim_key_vec[selection];

              if ( ImGui::BeginDragDropTarget() )
              {
                ImGuiDragDropFlags target_flags = 0;

                const ImGuiPayload *assetpayload = ImGui::AcceptDragDropPayload( "ASSETFILES", target_flags );
                if ( assetpayload )
                {
                  std::string assetpayload_n = *static_cast<std::string *>( assetpayload->Data );

                  if ( assetpayload_n.find( ".anim" ) != std::string::npos )
                    str = assetpayload_n.substr( 0, assetpayload_n.find_last_of( '.' ) );
                }
                ImGui::EndDragDropTarget();
              }
            }
            else if ( prop_type == rttr::type::get<Material *>() )
            {
              std::vector<std::string> shader_key_vec;
              shader_key_vec.push_back( " " );

              for ( auto &[key, data] : GetEnv().pManager->List<Shader>() )
                shader_key_vec.push_back( key.Key() );

              auto &str = value.get_value<Material *>()->m_ShaderKey;
              size_t selection = 0;

              for ( size_t i = 0; i < shader_key_vec.size(); i++ )
                if ( str == shader_key_vec[i] )
                  selection = i;

              size_t initial = selection;
              const char *shader_key = shader_key_vec[selection].c_str();

              if ( ImGui::BeginCombo( ( "##" + prop_name ).c_str(), shader_key ) )
              {
                for ( size_t i = 0; i < shader_key_vec.size(); i++ )
                {
                  const bool is_selected = ( selection == i );
                  if ( ImGui::Selectable( shader_key_vec[i].c_str(), is_selected ) )
                    selection = i;

                  if ( is_selected )
                    ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
              }
              if ( initial != selection )
                str = shader_key_vec[selection];
            }

            if ( prop_type == rttr::type::get<bool *>() )
            {
              ImGui::Dummy( { 0.0f, 2.0f } );
              pad = true;
            }
            else
              ImGui::Dummy( { 0.0f, 4.0f } );
          }
          if ( pad )
            ImGui::Dummy( { 0.0f, 6.0f } );
          else
            ImGui::Dummy( { 0.0f, 4.0f } );
          ImGui::Separator();
          ImGui::PopItemWidth();
        }
        ImGui::EndGroup();

        if ( ref.meta->bits != ComponentMeta::GetComponentMeta<EntityName>()->bits &&
             ref.meta->bits != ComponentMeta::GetComponentMeta<EntityType>()->bits &&
             ref.meta->bits != ComponentMeta::GetComponentMeta<Transform>()->bits &&
             ref.meta->bits != ComponentMeta::GetComponentMeta<Parent>()->bits )
        {
          ImGui::SameLine();
          ImGui::SetCursorPosX( ImGui::GetWindowContentRegionWidth() - 15.0f );
          Editor::font_awesome->Scale = 0.5f;
          ImGui::PushFont( Editor::font_awesome );
          ImGui::PushID( instance.get_type().get_name().to_string().c_str() );
          if ( ImGui::Button( ICON_FA_TIMES, { 30.0f,26.0f } ) )
          {
            RT_Reflect::RT_Destroy( em, { Editor::entity_id }, ref.meta->bits );
            ImGui::PopID();
            ImGui::PopFont();
            Editor::font_awesome->Scale = original;
            break;
          }
          ImGui::PopID();
          ImGui::PopFont();
          Editor::font_awesome->Scale = original;
        }

      }

    }
  }
  ImGui::End();
}
}
