#include "ImGui/Panels/PropertyInspectorPanel.h"
#include "ImGui/IconsFontAwesome5.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "ImGui/EditorDirectoryWatcher.h"
#include "Render/Texture.h"

#include "Assets/AssetManager.h"
#include "Reflect/Reflect.h"

namespace DeltaEngine
{

PropertyInspectorPanel::PropertyInspectorPanel( std::string str ) :
  IPanel( str )
{
  m_enabled = true;
  number_of_lines = 0;
  number_of_empty_lines = 0;
  number_of_frames = 0;
  m_texture_is_animation = false;
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
    ImGui::InputTextWithHint( "", "texture name", str1, IM_ARRAYSIZE( str1 ) );

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
            for (size_t i = 0; i < newFileName.length(); ++i)
            {
                fileName += newFileName[i];
            }
            //fileName(newFileName.begin(), newFileName.end());
            //std::string fileName(newFileName.begin(), newFileName.end());

            std::string png = "png";
            std::string info = "info";
            if (fileName.find(png) != 0 && fileName.find(png) != std::string::npos && fileName.find(info) == std::string::npos)
            {
                strcpy_s(str1, fileName.c_str());
                env.pManager->Load<Texture2D>(fileName, assetpayload_n);

                assetpayload_n += ".info";

                std::ifstream file(assetpayload_n.c_str());
                while (std::getline(file, stringcount))
                {
                    ++number_of_lines;
                    if (stringcount.empty())
                    {
                        ++number_of_empty_lines;
                    }
                }

                if (number_of_lines > 5)
                {
                    m_texture_is_animation = true;
                    number_of_lines = 0;
                    number_of_frames = number_of_empty_lines;
                    number_of_empty_lines = 0;
                }
                else
                {
                    m_texture_is_animation = false;
                    number_of_lines = 0;
                    number_of_frames = 0;
                    number_of_empty_lines = 0;
                }
            }
            else
            {
                std::string blank = "";
                strcpy_s(str1, blank.c_str());
            }
        }
        ImGui::EndDragDropTarget();
    }

    ImGui::SameLine();
    static int clicked = 0;
    if (ImGui::Button(ICON_FA_DOT_CIRCLE, { 20.0f, 18.0f }))
    {
        if (m_texture_is_animation)
            clicked++;
        else 
            clicked = 0;
    }
    if (m_texture_is_animation && clicked & 1)
    {
        ImGui::Begin("Frames", (bool*)true, ImGuiWindowFlags_AlwaysAutoResize);
        for (unsigned int i = 0; i < number_of_frames; ++i)
        {
            if (i % 2 != 0)
            {
                ImGui::SameLine();
            }
            Sprite frame = { fileName, i };
            uint64_t textureID = frame.GetTexture()->GetRendererID();
            ImGui::ImageButton(reinterpret_cast<void*>(textureID),
                ImVec2{ 60, 60 },
                ImVec2{ frame.GetOffset().x, frame.GetOffset().y },
                ImVec2{ frame.GetOffset().x + frame.GetTiling().x, frame.GetOffset().y + frame.GetTiling().y });
        }
        ImGui::End();
    }
    ImGui::SameLine();
    ImGui::Text("texture");

    static const char *components[] { " ", "name", "parent", "input",
      "transform", "rigidbody", "collider",
      "image","renderer2D", "text", "animator", "state",
      "entity_type", "ai", "health", "attack", "lifespan"};
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
          break;
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