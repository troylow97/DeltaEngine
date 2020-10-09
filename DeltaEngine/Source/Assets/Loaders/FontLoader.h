#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Font.h"

namespace DeltaEngine
{
  class FontLoader final : public AbstractLoader<Font>
  {
    void doLoad(AssetKey key) override
    {
    }

    void doLoad(AssetKey key, std::string_view str) override
    {
      Font* data = new Font{std::string(str)};
      if (data->characterInfo().empty())
        set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      else
        set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
