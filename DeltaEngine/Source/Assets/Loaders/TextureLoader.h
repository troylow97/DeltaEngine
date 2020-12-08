/**********************************************************************************
* \file   TextureLoader.h
* \brief  This file contain the implementation for TextureLoader
* \author Tan, Tong Wee, 100% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
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
      for (auto& file : FileUtils::FileList("Textures"))
        if (file.extension() == ".png" || file.extension() == ".jpg")
        {
          Texture2D* data = new Texture2D(file.generic_string());
          Set(file.generic_string().substr(0, file.generic_string().find_last_of('.')), data, AssetState::Final,
              AssetLifetime::Persistent);
        }

      for (auto& file : FileUtils::FileList("Tilemap"))
        if (file.extension() == ".png" || file.extension() == ".jpg")
        {
          Texture2D* data = new Texture2D(file.generic_string());
          Set(file.generic_string().substr(0, file.generic_string().find_last_of('.')), data, AssetState::Final,
              AssetLifetime::Persistent);
        }
    }

    void DoLoad(AssetKey key) override
    {
      if(FileUtils::FileExists( key.Key() + ".png" ))
      {
        Texture2D* data = new Texture2D(key.Key()+".png");
        Set( key, data, AssetState::Final, AssetLifetime::Persistent );
      }
      else if(FileUtils::FileExists( key.Key() + ".jpg" ))
      {
        Texture2D* data = new Texture2D(key.Key()+".jpg");
        Set( key, data, AssetState::Final, AssetLifetime::Persistent );
      }
    }

    void DoLoad(AssetKey key, std::string_view str) override
    {
      Texture2D* data = new Texture2D{std::string(str)};
      // To define how to handle failed Texture loading
      //if (data.characterInfo().empty())
      //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      //else

      Set(key, data, AssetState::Final, AssetLifetime::Persistent);
      if (!data)
      DeltaEngine_CORE_ERROR("FAILED");
    }
  };
} // namespace DeltaEngine
