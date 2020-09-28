#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Font.h"

namespace DeltaEngine
{

class FontLoader : public AbstractLoader<Font>
{

  void doLoad( AssetKey key ) override
  {

  }
  void doLoad( AssetKey key, std::string_view str ) override
  {
    std::cout << str << std::endl;
    Font data { std::string( str ) };
    if ( data.characterInfo().empty() )
      set( key, nullptr, AssetState::NotFound, AssetLifetime::Persistant );
    else
      set( key, data, AssetState::Final, AssetLifetime::Persistant );
  }

};

} // namespace DeltaEngine