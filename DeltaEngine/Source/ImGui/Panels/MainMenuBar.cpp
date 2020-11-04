#include "ImGui/Panels/MainMenuBar.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{

    MainMenuBar::MainMenuBar(std::string str) :
        IPanel(str)
    {

    }

    MainMenuBar::~MainMenuBar()
    {
        m_enabled = false;
    }

    bool MainMenuBar::Render(bool)
    {
        //ImGui::Begin(m_name.c_str());

        topLeft = ImGui::GetWindowContentRegionMin();
        bottomRight = ImGui::GetWindowContentRegionMax();

        topLeft.x += ImGui::GetWindowPos().x;
        topLeft.y += ImGui::GetWindowPos().y;
        bottomRight.x += ImGui::GetWindowPos().x;
        bottomRight.y += ImGui::GetWindowPos().y;

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("main"))
            {
                if (ImGui::MenuItem("new scene"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("load scene"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("save scene"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("quit"))
                {
                    /* Do stuff */
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("entity"))
            {
                if (ImGui::MenuItem("add entity"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("clone entity"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("save entity"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("delete entity"))
                {
                    /* Do stuff */
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("view"))
            {
                if (ImGui::MenuItem("hierarchy"))
                {
                    /* Do stuff */
                }
                if (ImGui::MenuItem("world"))
                {
                    /* Do stuff */
                }

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        return m_enabled;
    }

    //bool MainMenuBar::DraggedFileIn()
    //{
    //    if (InputManager::Get()->CurrentPosition().point_x >= topLeft.x && InputManager::Get()->CurrentPosition().point_x <= bottomRight.x
    //        && InputManager::Get()->CurrentPosition().point_y >= topLeft.y && InputManager::Get()->CurrentPosition().point_y <= bottomRight.y)
    //    {
    //        std::cout << "it is in mainmenubar panel!!!" << std::endl;
    //        return true;
    //    }
    //    return false;
    //}

    ImVec2 MainMenuBar::GetTopLeft()
    {
        return topLeft;
    }

    ImVec2 MainMenuBar::GetBottomRight()
    {
        return bottomRight;
    }
}