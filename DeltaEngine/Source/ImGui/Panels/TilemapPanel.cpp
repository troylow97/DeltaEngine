#include "ImGui/Panels/TilemapPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
    TilemapPanel::TilemapPanel(std::string str) :
        IPanel(str)
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
            std::cout << "it is in tileset panel!!!" << std::endl;
            return true;
        }
        return false;
    }

    bool TilemapPanel::Render(bool isdragged)
    {
        ImGui::Begin(m_name.c_str());
        auto& em = env.pECS->GetWorld().get_entity_manager();

        topLeft = ImGui::GetWindowContentRegionMin();
        bottomRight = ImGui::GetWindowContentRegionMax();

        topLeft.x += ImGui::GetWindowPos().x;
        topLeft.y += ImGui::GetWindowPos().y;
        bottomRight.x += ImGui::GetWindowPos().x;
        bottomRight.y += ImGui::GetWindowPos().y;

        //std::cout << "x is " << InputManager::Get()->CurrentPosition().point_x << " y is " << InputManager::Get()->CurrentPosition().point_y << std::endl;
        //std::cout << "render                   topLeft is " << topLeft.x << ", " << topLeft.y << std::endl;
        //std::cout << "render                   bottomRight is " << bottomRight.x << ", " << bottomRight.y << std::endl;

        if (isdragged)
        {
            DraggedFileIn();
        }

        ImGui::Text("Current tiles available:");



        ImGui::End();

        return m_enabled;
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