#include "Texture.h"
#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"
#include <stb_image.h>

namespace DeltaEngine
{
	Texture2D::Texture2D(const std::string filepath)
		: m_RendererID{ 0 }, m_Data{ nullptr }, m_Width{ 0 }, m_Height{ 0 }, texIndex{ 0 }
	{
		stbi_set_flip_vertically_on_load(0);
		
		GLCall(glGenTextures(1, &m_RendererID));
		GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

		int channels;

		m_Data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &channels, 0);

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

		textureInfo.push_back({
			Vector2(0, 0), Vector2(m_Width, m_Height), Vector2(0.5f, 0.5f), 0});
	}

	Texture2D::~Texture2D()
	{
		DeltaEngine_CORE_INFO("Deleting Texture");
		glDeleteTextures(1, &m_RendererID);
		DeltaEngine_CORE_INFO("Texture deleted");
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
	unsigned int Texture2D::GetRendererID() const
	{
		return m_RendererID;
	}

	void Texture2D::Slice(TextureInfo info)
	{
		textureInfo.push_back(info);
	}

	void Texture2D::SliceAll(unsigned int row, unsigned int column)
	{
		for (size_t x = 0; x < row; ++x)
		{
			for (size_t y = 0; y < column; ++y)
			{
				textureInfo.push_back({
					Vector2(m_Width / column * x, m_Height / row * y),
					Vector2(m_Width / column, m_Height / row),
					Vector2(0.5f, 0.5f),
					static_cast<unsigned int>(textureInfo.size()) });
			}
		}
	}
}
