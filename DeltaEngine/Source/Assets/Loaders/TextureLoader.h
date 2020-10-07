#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Texture.h"

namespace DeltaEngine
{
  class TextureLoader final : public AbstractLoader<Texture2D>
  {
    void doLoad(AssetKey key) override
    {
    }

    void doLoad(AssetKey key, std::string_view str) override
    {
      Texture2D* data = new Texture2D{std::string(str)};
      // To define how to handle failed Texture loading
      //if (data.characterInfo().empty())
      //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      //else
        set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
