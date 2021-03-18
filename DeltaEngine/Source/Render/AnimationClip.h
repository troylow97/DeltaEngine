/**********************************************************************************
* \file   AnimationClip.h
* \brief  The file contains interface of AnimationClip class.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "DEpch.h"
#include "Sprite.h"

namespace DeltaEngine
{
  class AnimationClip
  {
    std::string m_Name;
    unsigned totalFrames;
  public:
    unsigned fps;

    std::unordered_map<unsigned, Sprite> m_Sprites;

    bool looping;

    AnimationClip(std::string filepath = "Example.clip");
    unsigned GetTotalFrames() const;
    unsigned GetFps() const;
    Sprite GetSprite(unsigned frame);
    std::string GetName() const;
    static void CreateNew(
      std::string textureName,
      std::string filepath = "NewClip.clip",
      unsigned fps = 12,
      bool loop = true,
      unsigned start = 0,
      unsigned end = ~0);
  private:
    void LoadAnimation(std::string filepath);
    void UpdateAnimation(std::string filepath);
  };
}
