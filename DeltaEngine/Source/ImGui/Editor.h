#pragma once

#include <vector>
#include <memory>
#include "ImGui/Panels/IPanel.h"
#include "ECS/EntityManager.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
    class Editor
    {
        std::vector<std::unique_ptr<IPanel>> m_panels;
        const EntityManager& m_em;
        inline static bool drag;
    public:

        Editor() = delete;

        Editor(EntityManager& ref) : m_em{ ref }
        {
            m_panels.push_back(std::make_unique<WorldPanel>("World"));
        }

        void Render()
        {
            for (auto& ref : m_panels)
            {
                ref->Render(drag); // Update all the panels

            }
        }

        static void OnDragDrop(Event* e)
        {
            drag = true;
            
            //for (size_t i = 0; i < m_panels.size(); ++i)
            //{
            //
            //}
            //
            //
            //
            //
            //if (InputManager::Get()->CurrentPosition().point_x >= GetTopLeft().x && InputManager::Get()->CurrentPosition().point_x <= GetBottomRight().x
            //    && InputManager::Get()->CurrentPosition().point_y >= GetTopLeft().y && InputManager::Get()->CurrentPosition().point_y <= GetBottomRight().y)
            //{
            //    std::cout << "it is in world panel!!!" << std::endl;
            //    drag = true;
            //}
            //drag = false;

        }

    };

    //bool Editor::drag = false;
}
