/**********************************************************************************
* \file   AnimationControllerLoader.h
* \brief  This file contains the implementation for AnimationControllerLoader
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#pragma once

#include "Assets/AbstractLoader.h"
#include "Render/AnimationController.h"

namespace DeltaEngine
{
  class AnimationControllerLoader final : public AbstractLoader<AnimationController>
  {
    void DoLoad() override
    {
      for (auto& file : FileUtils::FileList("Animation"))
        if (file.extension() == ".anim")
        {
          AnimationController* data = new AnimationController(file.generic_string());
          Set(file.generic_string().substr(0, file.generic_string().find_last_of('.')),
              data, AssetState::Final, AssetLifetime::Persistent);
        }
    }

    void DoLoad(AssetKey key) override
    {
      if(FileUtils::FileExists( key.Key() + ".anim" ))
      {
        AnimationController* data = new AnimationController(key.Key() + ".anim");
        Set( key, data, AssetState::Final, AssetLifetime::Persistent );
      }
    }

    void DoLoad(AssetKey key, std::string_view str) override
    {
      AnimationController* data = new AnimationController{std::string(str)};
      // To define how to handle failed loading
      //if (data.characterInfo().empty())
      //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      //else
      Set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
