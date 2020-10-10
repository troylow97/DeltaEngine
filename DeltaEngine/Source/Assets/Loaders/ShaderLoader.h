#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Shader.h"

namespace DeltaEngine
{
class ShaderLoader final : public AbstractLoader<Shader>
{
  void DoLoad( AssetKey key ) override
  {}

  void DoLoad( AssetKey key, std::string_view str ) override
  {
    Shader *data = new Shader { std::string( str ) };
    // To define how to handle failed loading
    //if (data.characterInfo().empty())
    //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
    //else
    Set( key, data, AssetState::Final, AssetLifetime::Persistent );
  }
};
} // namespace DeltaEngine
