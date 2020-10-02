#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Shader.h"

namespace DeltaEngine
{
  class ShaderLoader final : public AbstractLoader<Shader>
  {
    void doLoad(AssetKey key) override
    {
    }

    void doLoad(AssetKey key, std::string_view str) override
    {
      Shader data{std::string(str)};
      // To define how to handle failed loading
      //if (data.characterInfo().empty())
      //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      //else
        set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
