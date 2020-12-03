#include "Core/TypeAlias.h"

#include <chrono>

namespace DeltaEngine
{
  class GameClock
  {
    using Nanoseconds = std::chrono::nanoseconds;
    using HighResClock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<HighResClock>;

  public:
    /*************************************************************
    * Constructor / Destructor
    *************************************************************/

    GameClock(f32 fps);
    ~GameClock() = default;

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
    f32 FrameCount() const;

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
    u32 m_timesteps;
  };
} // namespace DeltaEngine
