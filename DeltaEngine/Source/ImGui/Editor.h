#pragma once

#include <vector>

#include "ImGui/Panels/IPanel.h"
#include "ECS/EntityManager.h"
#include "Core/GlobalStruct.h"

namespace DeltaEngine
{
    class Editor
    {
        std::vector<DeltaEngine::IPanel*> m_panels;
        const EntityManager& m_em;
        inline static bool drag;
    public:

        Editor() = delete;

        Editor(EntityManager& ref) : m_em{ ref }
        {

        }

        void Render()
        {
            //for (auto& ref : m_panels)
            //{
            //    ref->Render(); // Update all the panels
            //    if (drag)
            //    {
            //        std::cout << "yes!" << std::endl;
            //    }
            //}
        }

        static void OnDragDrop(Event* e)
        {
            drag = true;
        }

    };

    //bool Editor::drag = false;
}
