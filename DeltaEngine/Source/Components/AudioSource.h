#pragma once

namespace DeltaEngine
{
  struct AudioSource
  {
    std::string clip {};
    unsigned id {0};
    float volume {1.0f};
    float pitch {0.0f};
    int loop {0};
    bool isLoop {false};
    bool isEvent {false};
    bool isStart { false };
  };
}