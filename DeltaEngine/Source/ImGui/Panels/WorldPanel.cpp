#include "ImGui/Panels/WorldPanel.h"
#include "Input/InputManager.h"

#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"

namespace DeltaEngine
{
    WorldPanel::WorldPanel(std::string str) :
        IPanel(str)
    {

    }

    WorldPanel::~WorldPanel()
    {
        m_enabled = false;
    }

    bool WorldPanel::DraggedFileIn()
    {
        if (InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
            && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y)
        {
            std::cout << "it is in world panel!!!" << std::endl;
            return true;
        }
        return false;
    }

    void WorldPanel::Render(bool isdragged)
    {
        ImGui::Begin( m_name.c_str(), &m_enabled);
        auto& em = env.pECS->GetWorld().GetEntityManager();

        topLeft = ImGui::GetWindowContentRegionMin();
        bottomRight = ImGui::GetWindowContentRegionMax();

        topLeft.x += ImGui::GetWindowPos().x;
        topLeft.y += ImGui::GetWindowPos().y;
        bottomRight.x += ImGui::GetWindowPos().x;
        bottomRight.y += ImGui::GetWindowPos().y;

        if (isdragged)
        {
            DraggedFileIn();
        }

        if (ImGui::TreeNode("Entities"))
        {
            for (size_t e_id = 0; e_id < em.GetEntities().size(); e_id++)
            {
              if (em.GetEntities()[e_id].chunk )
              {
                static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;

                ImGuiTreeNodeFlags node_flags = base_flags;
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

                ImGui::TreeNodeEx((void*)(intptr_t)e_id, node_flags, "entity %d", e_id);
                if (ImGui::IsItemClicked())
                {
                  InputManager::Get()->SetEntitySelected(true);
                  InputManager::Get()->SetEntityIDSelected(e_id);
                }
              }
               
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Environment"))
        {
            if (ImGui::TreeNode("NOTHING HERE"))
            {
                ImGui::Text("i told you there's nothing already lol");
                ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "pink");
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "yellow");
                ImGui::TextColored(ImVec4(0.25f, 0.875f, 0.8125f, 1.0f), "clara's fav color is turquoise");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNode("Segments"))
        {
            for (int i = 0; i < 6; i++)
            {
                static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;

                ImGuiTreeNodeFlags node_flags = base_flags;
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

                ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "fake one sike %d", i);
            }
            ImGui::TreePop();
        }
        ImGui::End();
    }

    ImVec2 WorldPanel::GetTopLeft()
    {
        return topLeft;
    }

    ImVec2 WorldPanel::GetBottomRight()
    {
        return bottomRight;
    }
}