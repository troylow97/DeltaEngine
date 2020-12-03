#include "SpriteEditorPanel.h"

#include "imgui.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Render/Texture.h"
#include "Input/Keys.h"

#include <string>

namespace DeltaEngine
{

static std::string selection;
static int id { -1 };
static bool loaded { false };
static ImVec2 initialPos { 0.f,0.f };
static ImVec2 previousPos { 0.f, 0.f };
static bool move { false };

SpriteEditorPanel::SpriteEditorPanel( std::string str )
  : IPanel( str )
{
  m_enabled = true;

}

SpriteEditorPanel::~SpriteEditorPanel()
{
  m_enabled = false;

}

void SpriteEditorPanel::Render()
{
  if ( ImGui::Begin( m_name.c_str(), &m_enabled, ImGuiWindowFlags_MenuBar ) )
  {

    previousPos = initialPos;
    initialPos = ImGui::GetMousePos();


    if ( ImGui::IsMouseDragging( 0 ) )
    {
      if ( id != -1 )
      {
        ImVec2 d = { initialPos.x - previousPos.x, initialPos.y - previousPos.y };
        if (move)
        {
          info[id].offset.x += d.x;
          info[id].offset.y += d.y;

        }
        else
          info[id].size.x += d.x;
          info[id].size.y += d.y;
      }
    }

    if (ImGui::IsMouseReleased(0) )
    {
      id = -1;
      move = false;
    }


    ImGui::InputText( "Texture Name", textureName, 128 );
    if ( ImGui::BeginDragDropTarget() )
    {
      ImGuiDragDropFlags target_flags = 0;

      const ImGuiPayload *assetpayload = ImGui::AcceptDragDropPayload( "ASSETFILES", target_flags );
      if ( assetpayload )
      {
        std::string assetpayload_n = *static_cast<std::string *>( assetpayload->Data );

          // find comments after start of argument
        std::size_t pos;
        pos = assetpayload_n.find( ".png" );
        if ( pos != std::string::npos )
          assetpayload_n.erase( pos );
        pos = assetpayload_n.find( "Texture" );
        assetpayload_n.erase( 0, pos );
        strcpy_s( textureName, assetpayload_n.c_str() );
      }
      ImGui::EndDragDropTarget();
    }

    Texture2D *texture = GetEnv().pManager->Get<Texture2D>( std::string( textureName ) );
    if ( ImGui::BeginMenuBar() )
    {
      if ( texture )
      {
        if ( ImGui::BeginMenu( "Auto Slicing" ) )
        {
          static const char *components[] { "Automatic", "Row by Column", "Nothing" };
          static int selected = 0;
          ImGui::Combo( "Slice Type", &selected, components, IM_ARRAYSIZE( components ) );
          if ( selected == 0 )
          {
            static float pivot[2] = { 0.5f, 0.5f };
            static bool overlap = false;
            ImGui::Checkbox( "Allow Overlapping Sprites", &overlap );
            ImGui::DragFloat2( "Pivot", pivot, 0.01f, 0.0f, 1.0f );
            if ( ImGui::Button( "Auto Slice" ) )
              texture->AutoSlice( Vector2( pivot[0], pivot[1] ), overlap );
          }
          if ( selected == 1 )
          {
            static int cr[2] = { 1, 1 };
            static float pivot[2] = { 0.5f, 0.5f };
            ImGui::DragInt2( "Row and Column", cr, 0.25f, 1, 128 );
            ImGui::DragFloat2( "Pivot", pivot, 0.01f, 0.0f, 1.0f );
            if ( ImGui::Button( "Auto Slice" ) )
              texture->SliceAll( cr[0], cr[1], Vector2( pivot[0], pivot[1] ) );
          }
          ImGui::EndMenu();
        }
        if ( ImGui::BeginMenu( "Texture Properties" ) )
        {
          static const char *wrapModes[] { "Repeat", "Mirror", "Clamp" };
          ImGui::Combo( "Texture Wrap Mode", (int *) &texture->wrapMode, wrapModes, IM_ARRAYSIZE( wrapModes ) );
          if ( ImGui::Button( "Save" ) )
            texture->InitTexture();
          ImGui::EndMenu();
        }
      }
      ImGui::EndMenuBar();
    }

    if ( texture )
    {
      uint64_t textureID = texture->GetRendererID();

      ImVec2 p = ImGui::GetCursorScreenPos();
      if ( ImGui::BeginChild( "Texture Editing" ), ImGuiWindowFlags_HorizontalScrollbar )
      {
        static float zoom = 1.0f;
        if ( ImGui::IsWindowHovered() )
          if ( ImGui::IsKeyDown( DEVK_LCTRL ) )
            zoom += 0.02f * ImGui::GetIO().MouseWheel;

        ImGui::Image(
          reinterpret_cast<void *>( textureID ),
          ImVec2 { texture->GetWidth() * zoom, texture->GetHeight() * zoom },
          ImVec2 { 0, 0 }, ImVec2 { 1, 1 }, ImVec4 { 1, 1, 1, 1 }, ImVec4 { 1, 1, 1, 1 } );
        float scrollY = ImGui::GetScrollY();
        if ( !loaded )
        {
          info = texture->textureInfo;
          loaded = true;
        }

        int i = 0;

        for ( const TextureInfo &detail : info )
        {
          ImVec2 min = {
            p.x + ( detail.offset.x ) * zoom + 1,
            p.y + ( detail.offset.y ) * zoom + 1 - scrollY };

          ImVec2 max = {
            p.x + ( detail.offset.x + detail.size.x ) * zoom + 1,
            p.y + ( detail.offset.y + detail.size.y ) * zoom + 1 - scrollY };

          ImVec2 center = {
            p.x + ( detail.offset.x + detail.pivot.x * detail.size.x ) * zoom + 1,
            p.y + ( detail.offset.y + detail.pivot.y * detail.size.y ) * zoom + 1 - scrollY };


          ImGui::GetWindowDrawList()->AddRectFilled( min, max, IM_COL32( 0, 255, 0, 34 ) );
          ImGui::GetWindowDrawList()->AddRect( min, max, IM_COL32( 0, 255, 0, 34 ) );
          ImGui::GetWindowDrawList()->AddCircle( center, 5.f, IM_COL32( 0, 255, 0, 51 ), 0, 2.0f );

          ImGui::SetCursorScreenPos( min );
          ImGui::InvisibleButton( ( "Area" + std::to_string( i ) ).c_str(), { max.x - min.x, max.y - min.y } );
          if(ImGui::IsItemClicked(0))
            id = i;

          //if ( ImGui::BeginDragDropSource( ImGuiDragDropFlags_SourceAllowNullID ) )
          //{
          //  selection.assign( texture->GetName() + '_' + std::to_string( i ) );
          //  ImGui::SetDragDropPayload( "SpriteEditorSource", &selection, sizeof( std::string ) );
          //  ImGui::Text( ( texture->GetName() + '_' + std::to_string( i ) ).c_str()  );
          //  ImGui::EndDragDropSource();
          //}
          ImGui::SetItemAllowOverlap();
          ImGui::SetCursorScreenPos( { center.x - 10.f, center.y - 10.f } );
          ImGui::Button( ( "##Center" + std::to_string( i ) ).c_str(), { 20.f, 20.f } );
          if(ImGui::IsItemClicked(0))
          {
            std::cout << "Inside" << std::endl;
            id = i;
            move = true;
          }

          ++i;
        }

      }
      ImGui::EndChild();
    }
  }
  ImGui::End();
}

}