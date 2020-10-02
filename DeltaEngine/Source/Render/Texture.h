#pragma once

#include "DEpch.h"
#include "Core/Math/Vector.h"

namespace DeltaEngine
{
	struct TextureInfo
	{
		Vector2 offset;
		Vector2 size;
		Vector2 pivot;
	};

	class Texture2D
	{
		unsigned int m_RendererID;
		unsigned char* m_Data;
		int m_Width, m_Height;
	public:
		Texture2D(const std::string filepath);
		~Texture2D();

		void Bind(unsigned int slot = 0) const;
		void Unbind() const;

		int GetWidth() const;
		int GetHeight() const;

		unsigned int GetRendererID() const;

		void Slice(TextureInfo info);
		void SliceAll(unsigned int row, unsigned int column);
	private:
		std::vector<TextureInfo> textureInfo;
		unsigned int texIndex;
	};
}
