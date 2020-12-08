/**********************************************************************************
* \file   ShaderLoader.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <cassert>
#include "Assets/AbstractLoader.h"
#include "Core/Utils/FileUtils.h"
#include "Render/Shader.h"

namespace DeltaEngine
{
  class ShaderLoader final : public AbstractLoader<Shader>
  {
    void DoLoad() override
    {
      for (auto& file : FileUtils::FileList("Shaders"))
        if (file.extension() == ".vs")
        {
          auto path = file.generic_string().substr(0, file.generic_string().find_last_of('.'));
          Shader* data = new Shader(path);
          auto name = path.substr(path.find_last_of('/') + 1, path.size() - path.find_last_of('/') - 1);
          Set(name, data, AssetState::Final, AssetLifetime::Persistent);
        }
    }

    void DoLoad(AssetKey key) override
    {
      if(FileUtils::FileExists( key.Key()+".fs") && FileUtils::FileExists( key.Key() + ".vs"))
      {
        Shader* data = new Shader(key.Key());
        Set( key, data, AssetState::Final, AssetLifetime::Persistent );
      }
    }

    void DoLoad(AssetKey key, std::string_view str) override
    {
      Shader* data = new Shader{std::string(str)};
      // To define how to handle failed loading
      //if (data.characterInfo().empty())
      //  set(key, nullptr, AssetState::NotFound, AssetLifetime::Persistent);
      //else
      Set(key, data, AssetState::Final, AssetLifetime::Persistent);
    }
  };
} // namespace DeltaEngine
