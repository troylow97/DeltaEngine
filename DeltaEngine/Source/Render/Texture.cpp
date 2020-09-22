#include "Texture.h"
#include "ErrorCheck.h"
#include "Core/Debug/Logger/Log.h"
#include <SOIL.h>

namespace DeltaEngine
{
	Texture2D::Texture2D(const std::string filepath) : m_RendererID{ 0 }, m_Data{ nullptr }, m_Width{ 0 }, m_Height{ 0 }, m_BPP{ 0 }
	{
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		m_Data = SOIL_load_image(filepath.c_str(), &m_Width, &m_Height, 0, SOIL_LOAD_RGBA);

		if (!m_Data)
			DeltaEngine_CORE_ERROR("ERROR: Couldn't create texture {}!", filepath);

		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data));
		GLCall(glGenerateMipmap(GL_TEXTURE_2D));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		if (m_Data)
			SOIL_free_image_data(m_Data);
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
}
