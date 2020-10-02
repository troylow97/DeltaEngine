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
    void TimeScale(const f32 scale);

    // GameClock state
    void Pause();
    void Resume();

    // called at start of loop
    void Update();

    // framecount
    u64 FrameCount() const;

    // getter for timescaled / real dt / fixed dt
    f64 DeltaTime() const;
    f64 RealDeltaTime() const;

    // ElapsedTime / Unscaled ElapsedTime
    f64 ElapsedTime() const;
    f64 UnscaledElapsedTime() const;

    // FPS
    f64 FrameRate() const;

  private:
    TimePoint _start;
    TimePoint _current;
    u64 _frame;
    f64 _elapsed;
    f64 _dt;
    f64 _g_elapsed;
    f64 _g_dt;
    f32 _timescale;
    bool _paused; 
    // memory size - 64
  };
} // namespace DeltaEngine