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
    std::string path {};
  };

  struct AudioConfig
  {
    std::vector<SoundConfig> sounds_config;
    std::vector<BankConfig> banks_config;
  };
}
