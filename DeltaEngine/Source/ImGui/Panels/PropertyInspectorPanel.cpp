#include "ImGui/Panels/PropertyInspectorPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "ImGui/EditorDirectoryWatcher.h"

#include "Assets/AssetManager.h"
#include "Reflect/Reflect.h"

namespace DeltaEngine
{

PropertyInspectorPanel::PropertyInspectorPanel( std::string str ) :
  IPanel( str )
{
  m_enabled = true;
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
    //44-124
    static char str1[128] = "";
    ImGui::SetNextItemWidth( 100 );
    ImGui::InputTextWithHint( "texture", "texture name", str1, IM_ARRAYSIZE( str1 ) );

    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;

        const ImGuiPayload* assetpayload = ImGui::AcceptDragDropPayload("ASSETFILES", target_flags);
        if (assetpayload)
        {
            std::string assetpayload_n = *(std::string*)(assetpayload->Data);
            std::wstring assetpayload_nws(assetpayload_n.begin(), assetpayload_n.end());
            std::size_t index = assetpayload_nws.find_last_of(L"/\\");
            std::wstring newFileName;
            for (size_t i = index + 1; i < assetpayload_nws.length(); ++i)
            {
                newFileName += assetpayload_nws[i];
            }
            std::string fileName(newFileName.begin(), newFileName.end());
            strcpy_s(str1, fileName.c_str());
        }
        ImGui::EndDragDropTarget();
    }

    //ImGui::SameLine();
    //static int clicked = 0;
    //ImGui::PushStyleColor( ImGuiCol_Button, ( ImVec4( 0.0f, 0.775f, 0.4125f, 1.0f ) ) );
    //ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ( ImVec4( 0.0f, 0.825f, 0.4125f, 1.0f ) ) );
    //ImGui::PushStyleColor( ImGuiCol_ButtonActive, ( ImVec4( 0.0f, 0.875f, 0.4125f, 1.0f ) ) );
    //if ( ImGui::Button( "Browse" ) )
    //{
    //  clicked++;
    //}
    //ImGui::PopStyleColor( 3 );
    //if ( clicked & 1 )
    //{
    //  ImGui::Begin( "Sprite Selection" );
    //  {
    //    ImGui::Text( "Character" );
    //
    //    std::vector<Sprite> spritelist;
    //    uint64_t textureID;
    //    Sprite running = { "run", 5 };
    //    Sprite idling = { "idle", 0 };
    //
    //    spritelist.push_back( running );
    //    spritelist.push_back( idling );
    //
    //    for ( int i = 0; i < spritelist.size(); ++i )
    //    {
    //      textureID = spritelist[i].GetTexture()->GetRendererID();
    //
    //      if ( ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
    //           ImVec2 { 32,32 },
    //           ImVec2 { spritelist[i].GetOffset().x, spritelist[i].GetOffset().y },
    //           ImVec2 { spritelist[i].GetOffset().x + spritelist[i].GetTiling().x, spritelist[i].GetOffset().y + spritelist[i].GetTiling().y } ) )
    //      {
    //        static int textureClicked = 0;
    //        textureClicked++;
    //        std::cout << "textureclicked is " << textureClicked << std::endl;
    //
    //        if ( textureClicked & 1 )
    //        {
    //          strcpy_s( str1, spritelist[i].GetName().c_str() );
    //        }
    //      }
    //      ImGui::SameLine();
    //    }
    //  }
    //  ImGui::Text( "" );
    //  ImGui::Text( "" );
    //  {
    //      // to show examples for more only
    //    ImGui::Text( "Background" );
    //
    //    Sprite bg = { "bg", 0 };
    //    uint64_t textureID = bg.GetTexture()->GetRendererID();
    //
    //    if ( ImGui::ImageButton( reinterpret_cast<void *>( textureID ),
    //         ImVec2 { 32,32 },
    //         ImVec2 { bg.GetOffset().x, bg.GetOffset().y },
    //         ImVec2 { bg.GetOffset().x + bg.GetTiling().x, bg.GetOffset().y + bg.GetTiling().y } ) )
    //    {
    //      static int textureClicked = 0;
    //      textureClicked++;
    //
    //      if ( textureClicked & 1 )
    //      {
    //        strcpy_s( str1, bg.GetName().c_str() );
    //      }
    //    }
    //    ImGui::SameLine();
    //  }
    //  ImGui::Text( "" );
    //  ImGui::Text( "" );
    //
    //  ImGui::End();
    //}
    //
    //ImGui::Text( "" );
    //ImGui::Text( "" );

    static const char *components[] { " ", "transform", "rigidbody", "collider", "input","ai","entity_type", "health", "attack", "lifespan"};
    static int selected = 0;
    ImGui::Combo( "Components", &selected, components, IM_ARRAYSIZE( components ) );
    if ( ImGui::Button( "Add Component" ) )
      if ( selected )
        RT_Reflect::RT_Setter( em, { index }, rttr::type::get_by_name( components[selected] ).get_metadata( "bits" ).to_uint64() );

    ImGui::Text( "" );
    if ( auto result = em.GetEntityArchetype( InputManager::Get()->EntityIDSelected() ); result != nullptr )
      for ( auto &ref : *result )
      {
        rttr::instance instance = em.GetComponent( { InputManager::Get()->EntityIDSelected() }, ref.meta->bits );
        /*
        auto& ref = em.GetComponent<Texture2D>(id);
        ref.key = dragged.file.key;
        */
        ImGui::Text( instance.get_type().get_name().to_string().c_str() );

        // 'x' button to remove component individually
        ImGui::PushID( instance.get_type().get_name().to_string().c_str() ); // ImGui uses the button's text as its identifier, thus need to create new ID stack
        ImGui::SameLine( ImGui::GetWindowWidth() - 30 );
        ImGui::PushStyleColor( ImGuiCol_Button, ( ImVec4( 0.73f, 0.25f, 0.25f, 1.0f ) ) );        // dull red
        ImGui::PushStyleColor( ImGuiCol_ButtonHovered, ( ImVec4( 0.85f, 0.33f, 0.35f, 1.0f ) ) ); // pale red
        ImGui::PushStyleColor( ImGuiCol_ButtonActive, ( ImVec4( 0.86f, 0.35f, 0.34f, 1.0f ) ) );  // pastel red
        if ( ImGui::Button( "x" ) )
        {
          RT_Reflect::RT_Destroy( em, { InputManager::Get()->EntityIDSelected() }, ref.meta->bits );
          ImGui::PopStyleColor( 3 );
          ImGui::PopID();
          continue;
        }
        ImGui::PopStyleColor( 3 );
        ImGui::PopID();

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

          if ( prop_type == rttr::type::get<float *>() )
            ImGui::DragFloat( prop_name.c_str(), ( value.get_value<float *>() ), 0.01f );
          if ( prop_type == rttr::type::get<int *>() )
            ImGui::DragInt( prop_name.c_str(), ( value.get_value<int *>() ) );
          else if ( prop_type == rttr::type::get<Vector2 *>() )
            ImGui::DragFloat2( prop_name.c_str(), (float *) ( value.get_value<Vector2 *>() ), 0.01f );
          else if ( prop_type == rttr::type::get<Vector3 *>() )
            ImGui::DragFloat3( prop_name.c_str(), (float *) ( value.get_value<Vector3 *>() ), 0.01f );
          else if ( prop_type == rttr::type::get<bool *>() )
            ImGui::Checkbox( prop_name.c_str(), value.get_value<bool *>() );
          else if ( prop_type == rttr::type::get<std::string *>() && instance.get_type() == rttr::type::get<AI>() || instance.get_type() == rttr::type::get<EntityType>() )
          {
            auto &str = *value.get_value<std::string *>();
            char buffer[256] {};
            strcpy_s( buffer, sizeof( buffer ), str.c_str() );
            if ( ImGui::InputText( prop_name.c_str(), buffer, sizeof( buffer ), ImGuiInputTextFlags_EnterReturnsTrue ) )
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
            ImGui::Combo( prop_name.c_str(), &current, c_ptr_vec.data(), c_ptr_vec.size() );
            const_cast<unsigned &>( value.get_wrapped_value<unsigned>() ) = current;
          }
          else if (prop_type == rttr::type::get<Color*>() )
          {
            auto& color = *value.get_value<Color *>();
            ImGui::DragFloat( "R", &color.r, 0.01f );
            ImGui::DragFloat( "G", &color.g, 0.01f );
            ImGui::DragFloat( "B", &color.b, 0.01f );
            ImGui::DragFloat( "A", &color.a, 0.01f );
          }
          else if ( prop_type == rttr::type::get<Sprite *>() )
          {
            std::vector<const char *> c_ptr_vec;
            c_ptr_vec.reserve( 128 );
            c_ptr_vec.push_back( " " );
            std::vector<std::string> str_vec;
            str_vec.reserve( 128 );
            for ( auto &[key, data] : GetEnv().pManager->List<Texture2D>() )
            {
              auto str = key.Key();
              for ( size_t i = 0; i < data->textureInfo.size(); i++ )
                c_ptr_vec.push_back( str_vec.emplace_back( std::string( str + '_' + std::to_string( i ) ) ).c_str() );
            }

            auto &sprite = *value.get_value<Sprite *>();
            int selection = 0;
            for ( int i = 0; i < c_ptr_vec.size(); i++ )
              if ( strcmp( ( sprite.m_Key + '_' + std::to_string( sprite.m_Index ) ).c_str(), c_ptr_vec[i] ) == 0 )
                selection = i;

            int initial = selection;

            ImGui::Combo( prop_name.c_str(), &selection, c_ptr_vec.data(), c_ptr_vec.size() );
            if ( initial != selection )
            {
              auto tmp = std::string( c_ptr_vec[selection] );
              auto offset = tmp.find_last_of( '_' );
              sprite.m_Key = tmp.substr( 0, offset );
              sprite.m_Index = std::stoi( tmp.substr( offset + 1 ) );
            }
          }
          else if ( prop_type == rttr::type::get<std::string *>() && instance.get_type() == rttr::type::get<Animator>() )
          {
            std::vector<const char *> c_ptr_vec;
            c_ptr_vec.reserve( 128 );
            c_ptr_vec.push_back( " " );
            std::vector<std::string> str_vec;
            str_vec.reserve( 128 );
            for ( auto &[key, data] : GetEnv().pManager->List<AnimationController>() )
                c_ptr_vec.push_back( str_vec.emplace_back( std::string( key.Key()) ).c_str() );

            auto &str = *value.get_value<std::string *>();
            int selection = 0;
            for ( int i = 0; i < c_ptr_vec.size(); i++ )
              if ( strcmp( str.c_str(), c_ptr_vec[i] ) == 0 )
                selection = i;

            int initial = selection;

            ImGui::Combo( prop_name.c_str(), &selection, c_ptr_vec.data(), c_ptr_vec.size() );
            if ( initial != selection )
              str= std::string( c_ptr_vec[selection] );
          }
          else if ( prop_type == rttr::type::get<Material *>() )
          {
            std::vector<const char *> c_ptr_vec;
            c_ptr_vec.reserve( 128 );
            c_ptr_vec.push_back( " " );
            std::vector<std::string> str_vec;
            str_vec.reserve( 128 );
            for ( auto &[key, data] : GetEnv().pManager->List<Shader>() )
              c_ptr_vec.push_back( str_vec.emplace_back( std::string( key.Key()) ).c_str() );

            auto &str = value.get_value<Material *>()->m_ShaderKey;
            int selection = 0;
            for ( int i = 0; i < c_ptr_vec.size(); i++ )
              if ( strcmp( str.c_str(), c_ptr_vec[i] ) == 0 )
                selection = i;

            int initial = selection;

            ImGui::Combo( prop_name.c_str(), &selection, c_ptr_vec.data(), c_ptr_vec.size() );
            if ( initial != selection )
              str= std::string( c_ptr_vec[selection] );
          }
 
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
}