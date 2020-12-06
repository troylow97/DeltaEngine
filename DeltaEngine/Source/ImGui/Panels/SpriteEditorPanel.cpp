#include "SpriteEditorPanel.h"

#include "imgui.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Render/Texture.h"
#include "Input/Keys.h"

#include <string>

namespace DeltaEngine
{


SpriteEditorPanel::SpriteEditorPanel( std::string str, Editor& e )
  : IPanel( str, e )
{
  m_enabled = false;

}

SpriteEditorPanel::~SpriteEditorPanel()
{
  m_enabled = false;
}

void SpriteEditorPanel::Enable()
{
  m_enabled = !m_enabled;
  loaded = false;
  info.clear();
}


void SpriteEditorPanel::Render()
{
  enum class MoveDir { None, Left, Right, Up, Down };
  static int selectedInfoID{ -1 };
  static int infoID{ -1 };
  static bool loaded{ false };
  static ImVec2 initialPos{ 0.f,0.f };
  static ImVec2 previousPos{ 0.f, 0.f };
  static ImVec2 spritePropsPos{ 0.f, 0.f };
  static bool hoveringSpriteProps{ false };
  static bool draggingSpriteProps{ false };
  static MoveDir moveDir{ MoveDir::None };

  if ( ImGui::Begin( m_name.c_str(), &m_enabled,
    ImGuiWindowFlags_MenuBar |
    ImGuiWindowFlags_NoNavInputs | 
    ImGuiWindowFlags_NoDocking) )
  {

    previousPos = initialPos;
    initialPos = ImGui::GetMousePos();

    if (!m_editor.textureKey.empty() )
    {
      ImGui::Text( ("Texture - " + m_editor.textureKey).c_str() );
      Texture2D* texture = GetEnv().pManager->Get<Texture2D>(m_editor.textureKey);

      if ( texture && ImGui::IsMouseDragging( 0 ) )
      {
        if (draggingSpriteProps)
        {
          ImVec2 d = { initialPos.x - previousPos.x, initialPos.y - previousPos.y };
          spritePropsPos.x += d.x;
          spritePropsPos.y += d.y;
        }

      if ( infoID >= 0 && infoID < info.size() )
      {
        ImVec2 d = { initialPos.x - previousPos.x, initialPos.y - previousPos.y };
        switch (moveDir)
        {
        case MoveDir::Left:
          info[infoID].offset.x += d.x;
          info[infoID].size.x -= d.x;
          if (info[infoID].offset.x < 0)
            info[infoID].size.x += info[infoID].offset.x;
          if (info[infoID].size.x < 1)
            info[infoID].offset.x += info[infoID].size.x - 1;
          break;
        case MoveDir::Right:
          info[infoID].size.x += d.x;
          break;
        case MoveDir::Up:
          info[infoID].offset.y += d.y;
          info[infoID].size.y -= d.y;
          if (info[infoID].offset.y < 0)
            info[infoID].size.y += info[infoID].offset.y;
          if (info[infoID].size.y < 1)
            info[infoID].offset.y += info[infoID].size.y - 1;
          break;
        case MoveDir::Down:
          info[infoID].size.y += d.y;
          break;
        }
        info[infoID].offset.x = Math::Clamp(info[infoID].offset.x, 0.f, 1.f * texture->GetWidth());
        info[infoID].offset.y = Math::Clamp(info[infoID].offset.y, 0.f, 1.f * texture->GetHeight());

          info[infoID].size.x = Math::Clamp(info[infoID].size.x, 1.f, 1.f * texture->GetWidth() - info[infoID].offset.x);
          info[infoID].size.y = Math::Clamp(info[infoID].size.y, 1.f, 1.f * texture->GetHeight() - info[infoID].offset.y);

          info[infoID].pivot.x = Math::Clamp01(info[infoID].pivot.x);
          info[infoID].pivot.y = Math::Clamp01(info[infoID].pivot.y);
        }
      }

      if (ImGui::IsMouseReleased(0) )
      {
        infoID = -1;
        moveDir = MoveDir::None;
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
        loaded = false;
        texture = GetEnv().pManager->Get<Texture2D>(std::string(textureName));
      }
      ImGui::EndDragDropTarget();
    }

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
              info = texture->AutoSlice( Vector2( pivot[0], pivot[1] ), overlap );
          }
          if ( selected == 1 )
          {
            static int cr[2] = { 1, 1 };
            static float pivot[2] = { 0.5f, 0.5f };
            ImGui::DragInt2( "Row and Column", cr, 0.25f, 1, 128 );
            ImGui::DragFloat2( "Pivot", pivot, 0.01f, 0.0f, 1.0f );
            if ( ImGui::Button( "Auto Slice" ) )
              info = texture->SliceAll(cr[0], cr[1], Vector2(pivot[0], pivot[1]));
          }
          ImGui::EndMenu();
        }
        if ( ImGui::BeginMenu( "Texture Properties" ) )
        {
          static const char *wrapModes[] { "Repeat", "Mirror", "Clamp" };
          ImGui::Combo( "Texture Wrap Mode", (int *) &texture->wrapMode, wrapModes, IM_ARRAYSIZE( wrapModes ) );
          if ( ImGui::Button( "Save" ) )
            texture->UpdateWrapMode(int(texture->wrapMode));
          ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Create Clip"))
        {
          static bool loop = true;
          static int fps = 12;
          ImGui::Checkbox("Loop", &loop);
          ImGui::DragInt("FPS", &fps, 0.01f, 1, 200);
          if (ImGui::Button("Apply Changes and Create Clip"))
            AnimationClip::CreateNew(texture->GetName(), texture->GetName() + ".clip", fps, loop);
          ImGui::EndMenu();
        }
        if ( ImGui::Button( "Apply Changes" ) )
          texture->Slice(info);
        if ( ImGui::Button( "Revert Changes" ) )
          loaded = false;
      }
      ImGui::EndMenuBar();
    }

      if ( texture )
      {
        uint64_t textureID = texture->GetRendererID();

        ImVec2 p = ImGui::GetCursorScreenPos();

        ImGui::SetCursorScreenPos(p);

      if ( ImGui::BeginChild( "Texture Editing" ), ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoMove )
      {
        static float zoom = 1.0f;
        if ( ImGui::IsWindowHovered() )
          if ( ImGui::IsKeyDown( DEVK_LCTRL ) )
            zoom += 0.02f * ImGui::GetIO().MouseWheel;

        if (selectedInfoID >= 0)
          if (ImGui::IsKeyPressed(DEVK_DELETE))
          {
            info.erase(info.begin() + selectedInfoID);
            selectedInfoID = -1;
          }

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

          if (!hoveringSpriteProps)
          {
            ImGui::SetCursorScreenPos(p);
            ImGui::InvisibleButton("##Area NULL", ImVec2{ 1.f * texture->GetWidth(), 1.f * texture->GetHeight() });
            if (ImGui::IsItemClicked(0))
              selectedInfoID = -1;
          }

          ImGui::SetCursorScreenPos(p);

          int i = 0;

          for ( TextureInfo &detail : info )
          {
            ImVec2 min = {
              p.x + ( detail.offset.x ) * zoom + 1,
              p.y + ( detail.offset.y ) * zoom + 1 - scrollY };

            ImVec2 max = {
              p.x + ( detail.offset.x + detail.size.x ) * zoom + 1,
              p.y + ( detail.offset.y + detail.size.y ) * zoom + 1 - scrollY };

            ImVec2 center = {
              p.x + ( detail.offset.x + 0.5f * detail.size.x ) * zoom + 1,
              p.y + ( detail.offset.y + 0.5f * detail.size.y ) * zoom + 1 - scrollY };

            ImVec2 piv = {
              p.x + ( detail.offset.x + detail.pivot.x * detail.size.x ) * zoom + 1,
              p.y + ( detail.offset.y + detail.pivot.y * detail.size.y ) * zoom + 1 - scrollY };

            if (!hoveringSpriteProps)
            {
              ImGui::SetCursorScreenPos(min);
              ImGui::SetItemAllowOverlap();
              ImGui::InvisibleButton(("##Area" + std::to_string(i)).c_str(), ImVec2{
                Math::Clamp(max.x - min.x, 1.0f, 1.f * texture->GetWidth()),
                Math::Clamp(max.y - min.y, 1.0f, 1.f * texture->GetHeight()) });
              if (ImGui::IsItemClicked(0))
                selectedInfoID = i;
            }

          if (selectedInfoID == i)
          {
            ImGui::GetWindowDrawList()->AddRectFilled(min, max, IM_COL32(0, 255, 0, 34));
            ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(0, 255, 0, 255));
            ImGui::GetWindowDrawList()->AddCircle(piv, 5.f, IM_COL32(0, 255, 255, 51), 0, 2.0f);

              //left
              ImGui::SetItemAllowOverlap();
              ImGui::SetCursorScreenPos({ min.x, center.y - 5.f });
              ImGui::Button(("##Left" + std::to_string(i)).c_str(), { 10.f, 10.f });
              if (ImGui::IsItemClicked(0))
                if (!hoveringSpriteProps)
                {
                  infoID = i;
                  moveDir = MoveDir::Left;
                }

              //right
              ImGui::SetItemAllowOverlap();
              ImGui::SetCursorScreenPos({ max.x - 10.f, center.y - 5.f });
              ImGui::Button(("##Right" + std::to_string(i)).c_str(), { 10.f, 10.f });
              if (ImGui::IsItemClicked(0))
                if (!hoveringSpriteProps)
                {
                  infoID = i;
                  moveDir = MoveDir::Right;
                }

              //up
              ImGui::SetItemAllowOverlap();
              ImGui::SetCursorScreenPos({ center.x - 5.f, min.y });
              ImGui::Button(("##Up" + std::to_string(i)).c_str(), { 10.f, 10.f });
              if (ImGui::IsItemClicked(0))
                if (!hoveringSpriteProps)
                {
                  infoID = i;
                  moveDir = MoveDir::Up;
                }

            //down
            ImGui::SetItemAllowOverlap();
            ImGui::SetCursorScreenPos({ center.x - 5.f, max.y - 10.f });
            ImGui::Button(("##Down" + std::to_string(i)).c_str(), { 10.f, 10.f });
            if (ImGui::IsItemClicked(0))
              if (!hoveringSpriteProps)
              {
                infoID = i;
                moveDir = MoveDir::Down;
              }
          }
          else
          {
            if (ImGui::IsWindowHovered() && ImGui::IsKeyDown(DEVK_LCTRL))
            {
              ImGui::GetWindowDrawList()->AddRectFilled(min, max, IM_COL32(0, 255, 0, 17));
              ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(0, 255, 0, 255));
              ImGui::GetWindowDrawList()->AddCircle(piv, 5.f, IM_COL32(0, 255, 255, 51), 0, 2.0f);
            }
            ImGui::GetWindowDrawList()->AddRectFilled(min, max, IM_COL32(255, 255, 255, 17));
            ImGui::GetWindowDrawList()->AddRect(min, max, IM_COL32(255, 255, 255, 34));
            ImGui::GetWindowDrawList()->AddCircle(piv, 5.f, IM_COL32(0, 255, 255, 17), 0, 2.0f);
          }

          ++i;
        }
        ImGui::SetItemAllowOverlap();
        if (selectedInfoID >= 0 && selectedInfoID < info.size())
        {
          p = ImVec2{ p.x + spritePropsPos.x, p.y + spritePropsPos.y };
          ImGui::SetCursorScreenPos(p);
          ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(51, 51, 51, 255));
          if (ImGui::BeginChild("Sprite Properties", ImVec2(300, 160), true))
          {
            static bool denyDrag = true;

              if ( !ImGui::IsWindowFocused() )
                denyDrag = true;

              hoveringSpriteProps = ImGui::IsWindowHovered();

              ImGui::SetCursorScreenPos(p);
              ImGui::InvisibleButton("##Sprite Properties Area", ImVec2(300, 160));
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = false;

              ImGui::SetCursorScreenPos(p);
              ImGui::Text("Sprite Properties");
              ImGui::Text("Sprite Name: %s", (texture->GetName() + "_" + std::to_string(selectedInfoID)).c_str());
              float offset[2] = { info[selectedInfoID].offset.x, info[selectedInfoID].offset.y };
              float size[2] = { info[selectedInfoID].size.x, info[selectedInfoID].size.y };
              float pivot[2] = { info[selectedInfoID].pivot.x, info[selectedInfoID].pivot.y };

              ImGui::DragFloat2("Offset", offset, 1.f);
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = true;

              ImGui::DragFloat2("Size", size, 1.f);
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = true;

              ImGui::DragFloat2("Pivot", pivot, 0.01f, 0.0f, 1.0f);
              ImGui::SetItemAllowOverlap();
              if (ImGui::IsItemClicked(0))
                denyDrag = true;

              info[selectedInfoID].offset = { offset[0], offset[1] };
              info[selectedInfoID].size = { size[0], size[1] };
              info[selectedInfoID].pivot = { pivot[0], pivot[1] };

              info[selectedInfoID].offset.x = Math::Clamp(info[selectedInfoID].offset.x, 0.f, 1.f * texture->GetWidth());
              info[selectedInfoID].offset.y = Math::Clamp(info[selectedInfoID].offset.y, 0.f, 1.f * texture->GetHeight());

              info[selectedInfoID].size.x = Math::Clamp(info[selectedInfoID].size.x, 1.f, 1.f * texture->GetWidth() - info[selectedInfoID].offset.x);
              info[selectedInfoID].size.y = Math::Clamp(info[selectedInfoID].size.y, 1.f, 1.f * texture->GetHeight() - info[selectedInfoID].offset.y);

              info[selectedInfoID].pivot.x = Math::Clamp01(info[selectedInfoID].pivot.x);
              info[selectedInfoID].pivot.y = Math::Clamp01(info[selectedInfoID].pivot.y);

            draggingSpriteProps = !denyDrag;
            if (ImGui::IsMouseReleased(0))
              denyDrag = true;
          }
          ImGui::EndChild();
          ImGui::PopStyleColor();
        }
      }
      ImGui::EndChild();
    }
    ImGui::End();
  }
}

}