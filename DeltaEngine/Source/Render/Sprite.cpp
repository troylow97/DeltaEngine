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
		return static_cast<unsigned int>(GetTexture()->GetSize(m_Index).x);
	}
	unsigned int Sprite::GetHeight()
	{
		return static_cast<unsigned int>(GetTexture()->GetSize(m_Index).y);
	}
	Vector2 Sprite::GetTiling()
	{
		return Vector2(
			1.0f * GetWidth() / GetTexture()->GetWidth(),
			1.0f * GetHeight() / GetTexture()->GetHeight()
		);
	}
	Vector2 Sprite::GetOffset()
	{
		return GetTexture()->GetOffset(m_Index);
	}
	Vector2 Sprite::GetPivot()
	{
		return GetTexture()->GetPivot(m_Index);
	}
	Texture2D* Sprite::GetTexture()
	{
		return GetEnv().pManager->get<Texture2D>(m_Key);
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
