#include "AnimationClip.h"
#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
	AnimationClip::AnimationClip(std::string filepath) : m_Name{ filepath }, totalFrames { 0 }, fps{ 12 }, loop{ true }
	{
		LoadAnimation(filepath);

		std::size_t pos;
		pos = m_Name.find(".");
		if (pos != std::string::npos)
			m_Name.erase(pos);
	}

	unsigned int AnimationClip::GetTotalFrames() const
	{
		return static_cast<unsigned int>(m_Sprites.size());
	}
	unsigned int AnimationClip::GetFps() const
	{
		return fps;
	}
	Sprite AnimationClip::GetSprite(unsigned int index) const
	{
		return m_Sprites[index];
	}
	std::string AnimationClip::GetName() const
	{
		return m_Name;
	}

	void AnimationClip::LoadAnimation(std::string filepath)
	{
		std::ifstream file;
		DeltaEngine_CORE_TRACE("Loading animation clip \"{}\"...", filepath.c_str());
		file.open((filepath).c_str());

		std::string str, spriteKey;
		unsigned int spriteIndex;

		if (file.is_open())
		{
			while (file.good())
			{
				file >> str;
				if (str[0] == '%')
					break;
				file >> str >> spriteKey;
				file >> str >> spriteIndex;
				m_Sprites.push_back(Sprite(spriteKey, spriteIndex));
			}
			file.close();
		}
		else
		{
			DeltaEngine_CORE_WARN("Animation clip file \"{}\" doesn't exist, creating automatically", filepath.c_str());

			UpdateAnimation(filepath);
		}
	}
	void AnimationClip::UpdateAnimation(std::string filepath)
	{
		std::ofstream file{ filepath.c_str() };
		if (file.is_open())
		{
			for (size_t i = 0; i < m_Sprites.size(); ++i)
			{
				file << m_Name << "_i_" << i << std::endl;
				file << "key " << m_Sprites[i].GetKey() << std::endl;
				file << "value " << m_Sprites[i].GetIndex() << std::endl;
				file << std::endl << "%" << std::endl;
			}
			file.close();
		}
		else
		{
			DeltaEngine_CORE_ERROR("Failed to create animation clip \"{}\"!", filepath);
		}
	}
}
