#pragma once

#include "DEpch.h"
#include "Core/Math/Vector.h"
#include "Texture.h"
#include "DE_API.h"

namespace DeltaEngine
{
	class DE_API Sprite
	{
		std::string m_Key;
	public:
		unsigned int m_Index;
		Sprite(std::string textureName = "", unsigned int index = 0);
		operator bool() const;
		unsigned int GetWidth();
		unsigned int GetHeight();
		Vector2 GetOffset();
		Vector2 GetTiling();
		Vector2 GetPivot();
		Texture2D* GetTexture();
		std::string GetName();
		std::string GetKey();
		unsigned int GetIndex();
	};
}
