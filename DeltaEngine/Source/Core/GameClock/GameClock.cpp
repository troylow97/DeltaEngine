#include "GameClock.h"

namespace DeltaEngine
{
  GameClock::GameClock()
      : _start(HighResClock::now()),
        _current(HighResClock::now()),
        _frame(0),
        _elapsed(0.0),
        _dt(0.0),
        _g_elapsed(0.0),
        _g_dt(0.0),
        _timescale(1.0f),
        _paused(false)
  {
  }

  f32 GameClock::TimeScale() const
  {
    return _timescale;
  }

  void GameClock::TimeScale(const f32 scale)
  {
    _timescale = scale;
  }

  void GameClock::Pause()
  {
    _paused = true;
  }

  void GameClock::Resume()
  {
    _paused = false;
  }

  void GameClock::Update()
  {
    _frame++;

    TimePoint now = HighResClock::now();
    Nanoseconds delta = std::chrono::duration_cast<Nanoseconds>(now - _current);
    _current = now;

    _dt = delta.count() * 1e-9;
    _elapsed += _dt;

    if (!_paused)
    {
      _g_dt = _dt * _timescale;
      _g_elapsed += _g_dt;
    }
  }

  u64 GameClock::FrameCount() const
  {
    return _frame;
  }

  f64 GameClock::DeltaTime() const
  {
    return _g_dt;
  }

  f64 GameClock::RealDeltaTime() const
  {
    return _dt;
  }

  f64 GameClock::ElapsedTime() const
  {
    return _g_elapsed;
  }

  f64 GameClock::UnscaledElapsedTime() const
  {
    return _elapsed;
  }

  f64 GameClock::FrameRate() const
  {
    return 1.0 / _dt;
  }

} // namespace DeltaEngine