#pragma once

#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
    struct CharacterInfo
    {
        unsigned int textureID = 0;
        Vector2 size = Vector2();
        Vector2 bearing = Vector2();
        unsigned int advance = 0;
    };

    class Font
    {
        std::map<char, CharacterInfo> m_CharacterInfo;
        unsigned int m_RendererID;
    public:
        Font(std::string filepath = "Fonts/Arial.ttf");
        std::map<char, CharacterInfo>& characterInfo();
        unsigned int GetID() const;
        static void Init();

    };
}
