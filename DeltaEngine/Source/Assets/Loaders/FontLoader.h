#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Font.h"

namespace DeltaEngine
{
class FontLoader final : public AbstractLoader<Font>
{
  void DoLoad( AssetKey key ) override
  {}

  void DoLoad( AssetKey key, std::string_view str ) override
  {
    Font *data = new Font { std::string( str ) };
    if ( data->characterInfo().empty() )
      Set( key, nullptr, AssetState::NotFound, AssetLifetime::Persistent );
    else
      Set( key, data, AssetState::Final, AssetLifetime::Persistent );
  }
};
} // namespace DeltaEngine
