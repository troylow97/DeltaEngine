#include "ImGui/Panels/TilemapPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
    TilemapPanel::TilemapPanel(std::string str) :
        IPanel(str),
        height{ 0 },
        width{ 0 }
    {

    }

    TilemapPanel::~TilemapPanel()
    {
        m_enabled = false;
    }

    bool TilemapPanel::DraggedFileIn()
    {
        if (InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
            && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y)
        {
            std::cout << "it is in world panel!!!" << std::endl;
            return true;
        }
        return false;
    }

    bool TilemapPanel::Render(bool isdragged)
    {
        ImGui::Begin(m_name.c_str());
        auto& em = env.pECS->GetWorld().get_entity_manager();

        ImVec2 renderSize = ImGui::GetContentRegionAvail();
        width = topLeft.x + renderSize.x;
        height = topLeft.y + renderSize.y;
        topLeft = ImGui::GetCursorScreenPos();
        bottomRight.x = topLeft.x + width;
        bottomRight.y = topLeft.y + height;

        if (isdragged)
        {
            DraggedFileIn();
        }

        ImGui::Text("Current tiles available:");



        ImGui::End();

        return m_enabled;
    }

    float TilemapPanel::GetHeight()
    {
        return height;
    }

    float TilemapPanel::GetWidth()
    {
        return width;
    }

    ImVec2 TilemapPanel::GetTopLeft()
    {
        return topLeft;
    }

    ImVec2 TilemapPanel::GetBottomRight()
    {
        return bottomRight;
    }
}