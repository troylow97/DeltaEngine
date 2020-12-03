#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Core/Utils/FileUtils.h"
#include "Render/Font.h"

namespace DeltaEngine
{
  class FontLoader final : public AbstractLoader<Font>
  {
    void DoLoad() override
    {
      for (auto& file : FileUtils::FileList("Fonts"))
        if (file.extension() == ".ttf")
        {
          Font* data = new Font(file.generic_string());
          Set(file.generic_string().substr(0, file.generic_string().find_last_of('.')),
              data, AssetState::Final, AssetLifetime::Persistent);
        }
    }

    void DoLoad(AssetKey key) override
    {
      if(FileUtils::FileExists( key.Key() + ".ttf" ))
      {
        Font* data = new Font(key.Key() + ".ttf");
        Set( key, data, AssetState::Final, AssetLifetime::Persistent );
      }
    }

    void DoLoad(AssetKey key, std::string_view str) override
    {
      Font* data = new Font{std::string(str)};
      if (data->characterInfo().empty())
        Set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      else
        Set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
