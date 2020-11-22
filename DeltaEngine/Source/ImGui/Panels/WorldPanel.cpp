#include "ImGui/Panels/WorldPanel.h"
#include "Input/InputManager.h"
#include "ImGui/Panels/LoggerPanel.h"
#include "Core/GlobalStruct.h"
#include "ECS/ECSModule.h"
#include "ImGui//Panels/LoggerPanel.h"
#include "Render/Camera.h"

namespace DeltaEngine
{
  WorldPanel::WorldPanel(std::string str) :
    IPanel(str)
  {
    m_enabled = true;
  }

  WorldPanel::~WorldPanel()
  {
    m_enabled = false;
  }

  bool WorldPanel::DraggedFileIn()
  {
    if (InputManager::Instance().CurrentPosition().point_x >= GetTopLeft().x && InputManager::Instance().
      CurrentPosition().point_x <= GetBottomRight().x
      && InputManager::Instance().CurrentPosition().point_y >= GetTopLeft().y && InputManager::Instance().
      CurrentPosition().point_y <= GetBottomRight().y)
    {
      std::cout << "it is in world panel!!!" << std::endl;
      return true;
    }
    return false;
  }

  Dragbox _drag_box;

  void WorldPanel::Render(bool isdragged)
  {
    ImGui::Begin(m_name.c_str(), &m_enabled);
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
        if (em.GetEntities()[e_id].chunk)
        {
          const auto &ref = em.GetComponent<EntityName>( { e_id } );
          std::string str = "Entity " + std::to_string( e_id );
          if ( !ref.name.empty() )
            str += " - " + ref.name;
          static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_None;

          ImGuiTreeNodeFlags node_flags = base_flags;
          node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet

          ImGui::TreeNodeEx((void*)static_cast<intptr_t>(e_id), node_flags, str.c_str());
          if (ImGui::IsItemClicked())
          {
            InputManager::Instance().SetEntitySelected(true);
            InputManager::Instance().SetEntityIDSelected(e_id);
            InputManager::Instance().SetCameraDragged(false);
            InputManager::Instance().SetEntityAllowDrag(true);
            InputManager::Instance().SetEntityChosenInViewport(false);
            InputManager::Instance().SetEntityChosenInWorld(true);

            auto& t = em.GetComponent<Transform>({ e_id });

            _drag_box.drag_box_transform = t;
            _drag_box.drag_box_transform.position = t.position;
            _drag_box.drag_box_transform.scale = { 0.3f, 0.3f, 0.0f };
          }
          if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
          {
            Camera::editorCamera->transform.position.x = em.GetComponent<Transform>({ e_id }).position.x;
            Camera::editorCamera->transform.position.y = em.GetComponent<Transform>({ e_id }).position.y;
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

        ImGui::TreeNodeEx((void*)static_cast<intptr_t>(i), node_flags, "fake one sike %d", i);
      }
      ImGui::TreePop();
    }
    ImGui::End();
  }
}
