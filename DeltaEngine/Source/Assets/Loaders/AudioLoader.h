/**********************************************************************************
* \file   AudioLoader.h
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
#include "Audio/AudioConfig.h"
#include "Audio/AudioEngine.h"
#include "Core/Utils/FileUtils.h"
#include "Core/Utils/Json/JsonFile.h"
#include "Render/Font.h"

namespace DeltaEngine
{
  class AudioLoader final : public AbstractLoader<Font>
  {
    void DoLoad() override
    {
      if ( FileUtils::FileExists( "Audio/Settings.cfg" ) )
      {
        JsonFile file;
        AudioConfig config;
        file.StartReader( "Audio/Settings.cfg" ).LoadObject( config ).EndReader();
        for ( const auto &ref : config.banks_config )
          if ( !AudioEngine::IsLoadedBank( ref.path ) )
            AudioEngine::LoadBank( ref.path, AUDIOENGINE_LOAD_BANK_NORMAL );

        for ( const auto &ref : config.sounds_config )
          AudioEngine::LoadSound( ref.path, ref.loop, ref.stream, ref.is3D );
      }
    }

    void DoLoad(AssetKey key) override
    {

    }

    void DoLoad(AssetKey key, std::string_view str) override
    {

    }
  };
} // namespace DeltaEngine
