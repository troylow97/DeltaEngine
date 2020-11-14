#include "GameClock.h"

#include "Core/Debugging/Profiler/Profiler.h"

namespace DeltaEngine
{
  GameClock::GameClock(f32 fps)
    : m_start(HighResClock::now()),
      m_current(HighResClock::now()),
      m_fixed_dt(1.0f / fps),
      m_elapsed(0.0),
      m_dt(0.0),
      m_g_elapsed(0.0),
      m_g_dt(0.0),
      m_timescale(1.0f),
      m_accumulator(0.0f),
      m_paused(false)
  {
  }

  f32 GameClock::TimeScale() const
  {
    return m_timescale;
  }

  void GameClock::TimeScale(const f32 scale)
  {
    m_timescale = scale;
  }

  void GameClock::Pause()
  {
    m_paused = true;
  }

  void GameClock::Resume()
  {
    m_paused = false;
  }

  void GameClock::Update()
  {
    TimePoint now = HighResClock::now();
    Nanoseconds delta = std::chrono::duration_cast<Nanoseconds>(now - m_current);
    m_current = now;

    m_dt = std::min(static_cast<f32>(delta.count() * 1e-9), m_fixed_dt);
    m_elapsed += m_dt;

    m_accumulator += m_dt;
    m_timesteps = 0;

    while (m_accumulator >= m_fixed_dt)
    {
      m_accumulator -= m_fixed_dt;
      ++m_timesteps;
    }

    if (!m_paused)
    {
      m_g_dt = m_dt * m_timescale;
      m_g_elapsed += m_g_dt;
    }
    else
      m_g_dt = 0.0f;

    Profiler::Instance().Record("Clock");
  }

  f32 GameClock::DeltaTime() const
  {
    return m_g_dt;
  }


  f32 GameClock::RealDeltaTime() const
  {
    return m_dt;
  }

  f32 GameClock::FixedDeltaTime() const
  {
    return m_fixed_dt * m_timescale;
  }

  f32 GameClock::ElapsedTime() const
  {
    return m_g_elapsed;
  }

  f32 GameClock::UnscaledElapsedTime() const
  {
    return m_elapsed;
  }

  f32 GameClock::FrameRate() const
  {
    return 1.0f / m_dt;
  }

  u32 GameClock::Timesteps() const
  {
    return m_timesteps;
  }
} // namespace DeltaEngine
