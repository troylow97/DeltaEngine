#pragma once

#include <string>

namespace DeltaEngine
{
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

	};
}
