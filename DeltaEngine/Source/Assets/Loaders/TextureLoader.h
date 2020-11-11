#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Render/Texture.h"
#include "Core/Utils/FileUtils.h"
namespace DeltaEngine
{
class TextureLoader final : public AbstractLoader<Texture2D>
{
  void DoLoad() override
  {
    for ( auto &file : FileUtils::FileList( "Textures" ))
      if (file.extension() == ".png" || file.extension() == ".jpg")
      {
        Texture2D *data = new Texture2D( file.generic_string() );
        Set( file.generic_string().substr(0, file.generic_string().find_last_of('.')), data, AssetState::Final, AssetLifetime::Persistent );

        DeltaEngine_CORE_TRACE( "Texture Key: {}", file.generic_string());

      }
  }

  void DoLoad( AssetKey key ) override
  {}

  void DoLoad( AssetKey key, std::string_view str ) override
  {
    Texture2D *data = new Texture2D { std::string( str ) };
    // To define how to handle failed Texture loading
    //if (data.characterInfo().empty())
    //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
    //else
    Set( key, data, AssetState::Final, AssetLifetime::Persistent );
  }
};
} // namespace DeltaEngine
