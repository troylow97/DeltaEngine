#pragma once
#include <cassert>
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

          DeltaEngine_CORE_TRACE("AnimationController Key: {}", file.generic_string());
        }
    }

    void DoLoad(AssetKey key) override
    {
      std::string str = key.Key() + ".anim";
      if(FileUtils::FileExists( str ))
      {
        AnimationController* data = new AnimationController(str);
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
