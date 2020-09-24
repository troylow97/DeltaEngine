#pragma once

#include "DEpch.h"
#include "Math/Vector.h"

namespace DeltaEngine
{
    struct CharacterInfo
    {
        unsigned int textureID;
        Vector2 size;
        Vector2 bearing;
        unsigned int advance;
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
