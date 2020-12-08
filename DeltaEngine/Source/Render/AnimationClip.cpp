/**********************************************************************************
* \file   AnimationClip.cpp
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AnimationClip.h"

#include <filesystem>

#include "ErrorCheck.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/DE_Math.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"

namespace DeltaEngine
{
  AnimationClip::AnimationClip(std::string filepath) : m_Name{filepath}, totalFrames{0}, fps{12}, looping{true}
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

  Sprite AnimationClip::GetSprite(unsigned int frame)
  {
    frame = Math::Clamp(frame + 1, 1, totalFrames);
    for (; frame > 0; --frame)
    {
      if (m_Sprites.count(frame - 1))
        return m_Sprites[frame - 1];
    }
    return Sprite();
  }

  std::string AnimationClip::GetName() const
  {
    return m_Name;
  }

  void AnimationClip::CreateNew(
    std::string textureName,
    std::string filepath,
    unsigned int fps,
    bool loop,
    unsigned int start,
    unsigned int end)
  {
    std::ofstream file{ filepath.c_str() };

    Texture2D* texture = GetEnv().pManager->Get<Texture2D>(std::string(textureName));

    if (start > end)
      end = start;
    if (start > texture->textureInfo.size())
      start = static_cast<unsigned>(texture->textureInfo.size() - 1);
    if (end > texture->textureInfo.size())
      end = static_cast<unsigned>(texture->textureInfo.size() - 1);

    if (file.is_open())
    {
      file << "frames " << end - start + 1 << std::endl;
      file << "fps " << fps << std::endl;
      file << "loop " << loop << std::endl << std::endl;

      for (size_t i = start; i <= end; ++i)
      {
        file << texture->GetName() << "_i_" << i << std::endl;
        file << "key " << texture->GetName() << std::endl;
        file << "value " << i << std::endl;
        file << "frame " << i - start << std::endl << std::endl;
      }
      file << "%" << std::endl;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_ERROR("Failed to create animation clip \"{}\"!", filepath);
    }
  }

  void AnimationClip::LoadAnimation(std::string filepath)
  {
    std::ifstream file;
    DeltaEngine_CORE_TRACE("Loading animation clip \"{}\"...", filepath.c_str());
    file.open((filepath).c_str());

    std::string str, spriteKey;
    unsigned int spriteIndex, frameNumber;

    if (file.is_open())
    {
      file >> str >> totalFrames;
      file >> str >> fps;
      file >> str >> looping;

      while (file.good())
      {
        file >> str;
        if (str[0] == '%')
          break;
        file >> str >> spriteKey;
        file >> str >> spriteIndex;
        file >> str >> frameNumber;
        m_Sprites[frameNumber] = Sprite(spriteKey, spriteIndex);
      }
      file.close();
    }
    else
    {
      DeltaEngine_CORE_WARN("Animation clip file \"{}\" doesn't exist, creating automatically", filepath.c_str());

      UpdateAnimation(filepath);
    }
    DeltaEngine_CORE_TRACE( "Animation clip {} was loaded successfully", filepath );
  }

  void AnimationClip::UpdateAnimation(std::string filepath)
  {
    std::ofstream file{filepath.c_str()};
    if (file.is_open())
    {
      file << "frames " << totalFrames << std::endl;
      file << "fps " << fps << std::endl;
      file << "loop " << looping << std::endl << std::endl;

      for (auto& [FrameNo, Value] : m_Sprites)
      {
        file << Value.m_Key << "_i_" << Value.m_Index << std::endl;
        file << "key " << Value.m_Key << std::endl;
        file << "value " << Value.m_Index << std::endl;
        file << "frame " << FrameNo << std::endl << std::endl;
      }
      file << "%" << std::endl;
      file.close();
    }
    else
    {
      DeltaEngine_CORE_ERROR("Failed to create animation clip \"{}\"!", filepath);
    }
  }
}
