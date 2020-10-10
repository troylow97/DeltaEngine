#include "DE_API.h"
#include "Core/TypeAlias.h"

#include <chrono>

namespace DeltaEngine
{
class DE_API GameClock
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
  void TimeScale( const f32 scale );

  // GameClock state
  void Pause();
  void Resume();

  // called at start of loop
  bool Update();

  // getter for timescaled / real dt / fixed dt
  f32 DeltaTime() const;
  f32 RealDeltaTime() const;

  // ElapsedTime / Unscaled ElapsedTime
  f32 ElapsedTime() const;
  f32 UnscaledElapsedTime() const;

  // FPS
  f32 FrameRate() const;

private:
#pragma warning(disable:4251)
  TimePoint m_start;
  TimePoint m_current;
#pragma warning(default:4251)
  f32 m_interval;
  f32 m_elapsed;
  f32 m_dt;
  f32 m_g_elapsed;
  f32 m_g_dt;
  f32 m_timescale;
  f32 m_accumulator;
  f32 m_seconds;
  u16 m_frame;
  u16 m_fps;
  bool m_paused;
  // memory size - 64
};
} // namespace DeltaEngine