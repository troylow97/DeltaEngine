#pragma once

namespace DeltaEngine
{
  struct AudioSource
  {
    std::string clip {};
    size_t id {0};
    float volume {1.0f};
    float pitch {0.0f};
    int loop {0};
    bool isLoop {false};
    bool isEvent {false};
    bool is3D { false };
    bool isStart { false };
    bool isPlaying { false };
    bool isPlayed { false };
  };
}