/**********************************************************************************
* \file   AudioConfig.h
* \brief  This file contains the definition  of Audio Configuration for the engine
*         The struct is used for serialization and deserialization of data
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once
#include <string>
#include <vector>

namespace DeltaEngine
{
  struct SoundConfig
  {
    std::string path{};
    bool loop{};
    bool stream{};
    bool is3D{};
  };

  struct BankConfig
  {
    std::string path{};
  };

  struct AudioConfig
  {
    std::vector<SoundConfig> sounds_config;
    std::vector<BankConfig> banks_config;
  };
}