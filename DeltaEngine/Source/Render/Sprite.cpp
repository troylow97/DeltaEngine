#include "Sprite.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
	Sprite::Sprite(std::string textureName, unsigned int index)
		: m_Key{ textureName }, m_Index{ index } { }

	Sprite::operator bool() const
	{
		return m_Key.length() > 0;
	}
	unsigned int Sprite::GetWidth()
	{
		if (GetTexture())
			return static_cast<unsigned int>(GetTexture()->GetSize(m_Index).x);
		return 0;
	}
	unsigned int Sprite::GetHeight()
	{
		if (GetTexture())
			return static_cast<unsigned int>(GetTexture()->GetSize(m_Index).y);
		return 0;
	}
	Vector2 Sprite::GetTiling()
	{
		if (GetTexture())
			return Vector2(
				1.0f * GetWidth() / GetTexture()->GetWidth(),
				1.0f * GetHeight() / GetTexture()->GetHeight()
			);
		return Vector2(1.0f, 1.0f);
	}
	Vector2 Sprite::GetOffset()
	{
		if (GetTexture())
			return GetTexture()->GetOffset(m_Index);
		return Vector2(0.0f, 0.0f);
	}
	Vector2 Sprite::GetPivot()
	{
		if (GetTexture())
			return GetTexture()->GetPivot(m_Index);
		return Vector2(0.5f, 0.5f);
	}
	Texture2D* Sprite::GetTexture()
	{
		if (m_Key.length() > 0)
			return GetEnv().pManager->Get<Texture2D>(m_Key);
		return nullptr;
	}

	std::string Sprite::GetName()
	{
		return m_Key + "_" + std::to_string(m_Index);
	}
	std::string Sprite::GetKey()
	{
		return m_Key;
	}
	unsigned int Sprite::GetIndex()
	{
		return m_Index;
	}
}
