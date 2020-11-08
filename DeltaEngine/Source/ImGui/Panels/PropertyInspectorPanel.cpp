#include "ImGui/Panels/PropertyInspectorPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{

PropertyInspectorPanel::PropertyInspectorPanel( std::string str ) :
  IPanel( str )
{

}

PropertyInspectorPanel::~PropertyInspectorPanel()
{
  m_enabled = false;
}

void PropertyInspectorPanel::Render( bool )
{
  ImGui::Begin( m_name.c_str(), &m_enabled );
  if ( InputManager::Get()->EntitySelected() )
  {
    auto &em = env.pECS->GetWorld().GetEntityManager();

    topLeft = ImGui::GetWindowContentRegionMin();
    bottomRight = ImGui::GetWindowContentRegionMax();

    topLeft.x += ImGui::GetWindowPos().x;
    topLeft.y += ImGui::GetWindowPos().y;
    bottomRight.x += ImGui::GetWindowPos().x;
    bottomRight.y += ImGui::GetWindowPos().y;

    size_t index = InputManager::Get()->EntityIDSelected();

    std::string text = "Edit Entity ";
    text += std::to_string( index );
    text += "'s Properties";
    ImGui::Text( text.c_str() );
    ImGui::Text( "" );

    static char str1[128] = "";
    ImGui::SetNextItemWidth( 100 );
    ImGui::InputTextWithHint( "texture", "texture name", str1, IM_ARRAYSIZE( str1 ) );
    ImGui::SameLine();
    static int clicked = 0;
    ImGui::PushStyleColor( ImGuiCol_Button, ( ImVec4( 0.0f, 0.775f, 0.4125f, 1.0f ) ) );
    ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ( ImVec4( 0.0f, 0.825f, 0.4125f, 1.0f ) ) );
    ImGui::PushStyleColor( ImGuiCol_ButtonActive, ( ImVec4( 0.0f, 0.875f, 0.4125f, 1.0f ) ) );
    if ( ImGui::Button( "Browse" ) )
    {
      clicked++;
    }
    ImGui::PopStyleColor( 3 );
    if ( clicked & 1 )
    {
      ImGui::Begin( "Sprite Selection" );
      {
        ImGui::Text( "Character" );

        std::vector<Sprite> spritelist;
        uint64_t textureID;
        Sprite running = { "run", 5 };
        Sprite idling = { "idle", 0 };

        spritelist.push_back( running );
        spritelist.push_back( idling );

        for ( int i = 0; i < spritelist.size(); ++i )
        {
          textureID = spritelist[i].GetTexture()->GetRendererID();

          if ( ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
               ImVec2 { 32,32 },
               ImVec2 { spritelist[i].GetOffset().x, spritelist[i].GetOffset().y },
               ImVec2 { spritelist[i].GetOffset().x + spritelist[i].GetTiling().x, spritelist[i].GetOffset().y + spritelist[i].GetTiling().y } ) )
          {
            static int textureClicked = 0;
            textureClicked++;
            std::cout << "textureclicked is " << textureClicked << std::endl;

            if ( textureClicked & 1 )
            {
              strcpy( str1, spritelist[i].GetName().c_str() );
            }
          }
          ImGui::SameLine();
        }
      }
      ImGui::Text( "" );
      ImGui::Text( "" );
      {
          // to show examples for more only
        ImGui::Text( "Background" );

        Sprite bg = { "bg", 0 };
        uint64_t textureID = bg.GetTexture()->GetRendererID();

        if ( ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
             ImVec2 { 32,32 },
             ImVec2 { bg.GetOffset().x, bg.GetOffset().y },
             ImVec2 { bg.GetOffset().x + bg.GetTiling().x, bg.GetOffset().y + bg.GetTiling().y } ) )
        {
          static int textureClicked = 0;
          textureClicked++;

          if ( textureClicked & 1 )
          {
            strcpy( str1, bg.GetName().c_str() );
          }
        }
        ImGui::SameLine();
      }
      ImGui::Text( "" );
      ImGui::Text( "" );

      ImGui::End();
    }

    ImGui::Text( "" );
    ImGui::Text( "" );

    static const char *components[] { " ", "transform", "rigidbody", "collider", "input" };
    static int selected = 0;
    ImGui::Combo( "Components", &selected, components, IM_ARRAYSIZE( components ) );
    if ( ImGui::Button( "Add Component" ) )
    {
      if ( selected != 0 )
        RT_Reflect::RT_Setter( em, { index }, rttr::type::get_by_name( components[selected] ).get_metadata( "bits" ).to_uint64() );
    }

    ImGui::Text( "" );
    ImGui::Text( "" );
    if ( auto result = em.GetEntityArchetype( InputManager::Get()->EntityIDSelected() ); result != nullptr )
      for ( auto &ref : *result )
      {
        rttr::instance &instance = em.GetComponent( { InputManager::Get()->EntityIDSelected() }, ref.meta->bits );

        ImGui::Text( instance.get_type().get_name().to_string().c_str() );
        auto properties = instance.get_type().get_properties();
        for ( auto property : properties )
        {
          rttr::variant value = property.get_value( instance );
          if ( !value )
            continue;

          if ( property.get_metadata( "NO_EDITOR" ) )
            continue;

          auto prop_type = property.get_type();
          auto prop_name = property.get_name().to_string();

          if ( prop_type == rttr::type::get<float*>() )
            ImGui::DragFloat( prop_name.c_str(), ( value.get_value<float *>() ), 0.01f );
          else if ( prop_type == rttr::type::get<Vector2*>())
            ImGui::DragFloat2( prop_name.c_str(), (float *) ( value.get_value<Vector2 *>() ), 0.01f );
          else if ( prop_type == rttr::type::get<Vector3*>())
            ImGui::DragFloat3( prop_name.c_str(), (float *) ( value.get_value<Vector3 *>() ), 0.01f );
          else if ( prop_type == rttr::type::get<bool*>() )
            ImGui::Checkbox( prop_name.c_str(),  value.get_value<bool *>()  );

        }
      }

  }
  ImGui::End();
}

//bool PropertyInspectorPanel::DraggedFileIn()
//{
//    if (InputManager::Get()->CurrentPosition().point_x >= topLeft.x && InputManager::Get()->CurrentPosition().point_x <= bottomRight.x
//        && InputManager::Get()->CurrentPosition().point_y >= topLeft.y && InputManager::Get()->CurrentPosition().point_y <= bottomRight.y)
//    {
//        std::cout << "it is in propertyinspector panel!!!" << std::endl;
//        return true;
//    }
//    return false;
//}

ImVec2 PropertyInspectorPanel::GetTopLeft()
{
  return topLeft;
}

ImVec2 PropertyInspectorPanel::GetBottomRight()
{
  return bottomRight;
}
}