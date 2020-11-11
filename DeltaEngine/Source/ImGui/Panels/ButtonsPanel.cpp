#include "ImGui/Panels/ButtonsPanel.h"
#include "ImGui/IconsFontAwesome5.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
//#include "Render/Texture.h"
//#include "Assets/AssetManager.h"
//#include "ECS/ECSModule.h"
//#include "Core/Utils/FileUtils.h"

namespace DeltaEngine
{
    ButtonsPanel::ButtonsPanel(std::string str) :
        IPanel(str)
    {
        m_enabled = true;
    }

    ButtonsPanel::~ButtonsPanel()
    {
        m_enabled = false;
    }

    bool ButtonsPanel::DraggedFileIn()
    {
        if (InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
            && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y)
        {
            std::cout << "it is in button panel!!!" << std::endl;
            return true;
        }
        return false;
    }

    void ButtonsPanel::Render(bool isdragged)
    {
        ImGui::Begin(m_name.c_str(), &m_enabled, ImGuiWindowFlags_NoScrollbar);

        topLeft = ImGui::GetWindowContentRegionMin();
        bottomRight = ImGui::GetWindowContentRegionMax();

        topLeft.x += ImGui::GetWindowPos().x;
        topLeft.y += ImGui::GetWindowPos().y;
        bottomRight.x += ImGui::GetWindowPos().x;
        bottomRight.y += ImGui::GetWindowPos().y;

        ImVec2 ButtonPos1 = { ImGui::GetWindowSize().x * 0.485f, ImGui::GetWindowSize().y * 0.3f };
        ImVec2 ButtonPos2 = { ImGui::GetWindowSize().x * 0.515f, ImGui::GetWindowSize().y * 0.3f };
        ImGui::SetCursorPos(ButtonPos1);
        if (ImGui::Button(ICON_FA_PLAY, { 25.0f, 22.0f }))
        {
            std::cout << "play is clicked" << std::endl;
        }
        ImGui::SetCursorPos(ButtonPos2);
        if (ImGui::Button(ICON_FA_PAUSE, { 25.0f, 22.0f }))
        {
            std::cout << "pause is clicked" << std::endl;
        }

        ImGui::End();
    }

    ImVec2 ButtonsPanel::GetTopLeft()
    {
        return topLeft;
    }

    ImVec2 ButtonsPanel::GetBottomRight()
    {
        return bottomRight;
    }
}