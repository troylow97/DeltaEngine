#pragma once

#include <vector>
#include <memory>
#include "ImGui/Panels/IPanel.h"
#include "ECS/EntityManager.h"
#include "Core/GlobalStruct.h"
#include "ImGui\Panels\MainMenuBar.h"
#include "ImGui\Panels\PropertyInspectorPanel.h"
#include "ImGui\Panels\TilemapPanel.h"

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
            m_panels.push_back(std::make_unique<MainMenuBar>("MainMenu"));
            //m_panels.push_back(std::make_unique<PropertyInspectorPanel>("Property Inspector"));
            m_panels.push_back(std::make_unique<TilemapPanel>("Tilemap"));
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

        }

    };
}
