#pragma once

#include <vector>

#include "ImGui/Panels/IPanel.h"
#include "ECS/EntityManager.h"

namespace DeltaEngine
{
    class Editor
    {
        std::vector<DeltaEngine::IPanel*> m_panels;
        const EntityManager& m_em;
    public:

        Editor() = delete;

        Editor(EntityManager& ref) : m_em{ ref }
        {

        }

        void Render()
        {
            for (auto& ref : m_panels)
            {
                ref->Render(); // Update all the panels
            }
        }
    };
}
