#include "Texture.h"
#include "ErrorCheck.h"
#include "Core/Debug/Logger/Log.h"
#include <stb_image.h>

namespace DeltaEngine
{
	Texture2D::Texture2D(const std::string filepath) : m_RendererID{ 0 }, m_Data{ nullptr }, m_Width{ 0 }, m_Height{ 0 }
	{
		stbi_set_flip_vertically_on_load(0);
		
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		int channels;

		m_Data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &channels, 0);;

		if (!m_Data)
			DeltaEngine_CORE_ERROR("ERROR: Couldn't create texture {}!", filepath);

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_Data)
			stbi_image_free(m_Data);
	}

	Texture2D::~Texture2D()
	{
		GLCall(glDeleteTextures(1, &m_RendererID));
	}

	void Texture2D::Bind(unsigned int slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
	}

	void Texture2D::Unbind() const
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
	}

	int Texture2D::GetWidth() const
	{
		return m_Width;
	}
	int Texture2D::GetHeight() const
	{
		return m_Height;
	}
}
