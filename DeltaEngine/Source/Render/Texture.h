#pragma once

#include <string>

class Texture2D
{
	unsigned int m_RendererID;
	unsigned char* m_Data;
	int m_Width, m_Height, m_BPP;
public:
	Texture2D(const std::string filepath);
	~Texture2D();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

};
