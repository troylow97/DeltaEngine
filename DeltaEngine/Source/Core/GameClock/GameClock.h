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

  GameClock();
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
  f32 UnscaledDeltaTime() const;

  // ElapsedTime / Unscaled ElapsedTime
  f32 ElapsedTime() const;
  f32 UnscaledElapsedTime() const;

  // FPS
  u32 FrameRate() const;

  bool update {false};
private:
  TimePoint m_current;
  u32 m_fps;
  u32 m_frame;
  f32 m_dt;
  f32 m_g_dt;
  f32 m_elapsed;
  f32 m_g_elapsed;
  f32 m_timescale;
  f32 m_accumulator;
  f32 m_frameDuration;
  f32 m_interval;
  bool m_paused;
};

} // namespace DeltaEngine