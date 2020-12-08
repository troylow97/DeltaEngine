/**********************************************************************************
* \file   AnimationClipLoader.h
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
#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/AnimationClip.h"

namespace DeltaEngine
{
  class AnimationClipLoader final : public AbstractLoader<AnimationClip>
  {
    void DoLoad() override
    {
      for (auto& file : FileUtils::FileList("Clip"))
        if (file.extension() == ".clip")
        {
          AnimationClip* data = new AnimationClip(file.generic_string());
          Set(file.generic_string().substr(0, file.generic_string().find_last_of('.')),
              data, AssetState::Final, AssetLifetime::Persistent);
        }
    }

    void DoLoad(AssetKey key) override
    {
      if(FileUtils::FileExists(  key.Key() + ".clip" ))
      {
        AnimationClip* data = new AnimationClip( key.Key() + ".clip");
        Set( key, data, AssetState::Final, AssetLifetime::Persistent );
      }
    }

    void DoLoad(AssetKey key, std::string_view str) override
    {
      AnimationClip* data = new AnimationClip{std::string(str)};
      // To define how to handle failed loading
      //if (data.characterInfo().empty())
      //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      //else
      Set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
