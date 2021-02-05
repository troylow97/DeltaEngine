/**********************************************************************************
* \file   EngineClock.h
* \brief  This file contains the definition of the engine's clock
*
*         The engine clock is written to support the following:
*         - Timescaling
*         - Timestepping
*         - unscaled / scaled / fixed unscaled / fixed scaled deltaTime 
*         - unscaled / scaled / fixed unscaled / fixed scaled Time tracking
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "Core/TypeAlias.h"

#include <chrono>

namespace DeltaEngine
{
  class EngineClock
  {
    using Nanoseconds = std::chrono::nanoseconds;
    using HighResClock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<HighResClock>;

  public:
    /*************************************************************
    * Constructor / Destructor
    *************************************************************/

    EngineClock(f32 fps);
    ~EngineClock() = default;

    /*************************************************************
    * Interface
    *************************************************************/

    // timescale setter / getter
    f32 TimeScale() const;
    void TimeScale(f32 scale);

    // called at start of loop
    void Update();

    // getter for timescaled / real dt / fixed dt
    f32 DeltaTime() const;
    f32 FixedDeltaTime() const;
    f32 FixedUnscaledDeltaTime() const;
    f32 UnscaledDeltaTime() const;

    // ElapsedTime / Unscaled ElapsedTime
    f32 Time() const;
    f32 FixedTime() const;
    f32 FixedUnscaledTime() const;
    f32 UnscaledTime() const;

    // FPS
    u32 FrameCount() const;

    // TimeStep
    u32 Timesteps() const;

  private:
    // FrameTime
    TimePoint m_start;
    TimePoint m_current;
    // DT
    f32 m_dt;
    f32 m_fixed_dt;
    // Time
    f32 m_time;;
    f32 m_fixed_time;
    f32 m_fixed_unscaled_time;
    f32 m_unscaled_time;
    // Controls
    f32 m_timescale;
    f32 m_accumulator;
    f32 m_frames_tracker;
    u32 m_timesteps;
    u32 m_framecounts;
    u32 m_frames;
  };
} // namespace DeltaEngine
