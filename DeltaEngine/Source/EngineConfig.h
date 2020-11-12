#pragma once
#include <string>

struct EngineConfig
{
  std::string win_name{};
  int width{0};
  int height{0};
  float fps{0.0f};
  bool fullscreen{false};


};