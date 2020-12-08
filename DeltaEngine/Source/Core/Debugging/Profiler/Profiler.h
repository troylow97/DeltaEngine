/**********************************************************************************
* \file   Profiler.h
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
#include "Core/Containers/Singleton.h"
#include <chrono>
#include <string>
#include <vector>

namespace DeltaEngine
{
  class Profiler : public Singleton<Profiler>
  {
    friend class Singleton<Profiler>;
    Profiler() = default;
    ~Profiler() = default;

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
