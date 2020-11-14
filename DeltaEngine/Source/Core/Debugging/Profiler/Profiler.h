#pragma once
#include "Core/Containers/Singleton.h"
#include <chrono>
#include <string>
#include <vector>

namespace DeltaEngine
{
  class Profiler : public Singleton<Profiler>
  {
    using Nanoseconds = std::chrono::nanoseconds;
    using HighResClock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<HighResClock>;

    struct Details
    {
      std::string name{};
      Nanoseconds delta{};
    };

    using Buffer = std::vector<Details>;


    TimePoint m_start{};
    TimePoint m_prev{};
    Buffer m_buffer[2]{};
    Nanoseconds m_delta[2]{};
    unsigned m_index{0};

  public:
    void FrameStart();
    void FrameEnd();

    void Record(std::string_view);

    void Print();
  };
}
