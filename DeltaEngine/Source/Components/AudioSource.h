#pragma once

namespace DeltaEngine
{
  struct AudioSource
  {
    unsigned id {0};
    float volume {1.0f};
    float pitch {0.0f};
    int loop {0};
    bool isLoop {false};
    bool isEvent {false};
  };
}