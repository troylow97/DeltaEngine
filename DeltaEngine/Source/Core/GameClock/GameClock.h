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

  GameClock( f32 fps );
  ~GameClock() = default;

  /*************************************************************
  * Interface
  *************************************************************/

  // timescale setter / getter
  f32 TimeScale() const;
  void TimeScale( const f32 scale );

  // GameClock state
  void Pause();
  void Resume();

  // called at start of loop
  void Update();

  // getter for timescaled / real dt / fixed dt
  f32 DeltaTime() const;
  f32 RealDeltaTime() const;
  f32 FixedDeltaTime() const;

  // ElapsedTime / Unscaled ElapsedTime
  f32 ElapsedTime() const;
  f32 UnscaledElapsedTime() const;

  // FPS
  f32 FrameRate() const;

  // TimeStep
  u32 Timesteps() const;

private:
  TimePoint m_start;
  TimePoint m_current;
  f32 m_fixed_dt;
  f32 m_elapsed;
  f32 m_dt;
  f32 m_g_elapsed;
  f32 m_g_dt;
  f32 m_timescale;
  f32 m_accumulator;
  u32 m_timesteps;
  bool m_paused;
};
} // namespace DeltaEngine