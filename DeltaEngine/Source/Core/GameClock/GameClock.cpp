#include "GameClock.h"

#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  GameClock::GameClock(f32 fps)
    : m_start(HighResClock::now()),
      m_current(HighResClock::now()),
      m_dt(0.0f),
      m_fixed_dt(1.0f / fps),
      m_time(0.0f),
      m_fixed_time(0.0f),
      m_fixed_unscaled_time(0.0),
      m_unscaled_time(0.0),
      m_timescale(1.0f),
      m_accumulator(0.0f),
      m_timesteps(0)
  {
    DeltaEngine_CORE_INFO("Game clock initialized, FPS Cap - {} ", fps);
  }

  f32 GameClock::TimeScale() const
  {
    return m_timescale;
  }

  void GameClock::TimeScale(const f32 scale)
  {
    m_timescale = scale;
  }

  void GameClock::Update()
  {
    TimePoint now = HighResClock::now();
    Nanoseconds delta = std::chrono::duration_cast<Nanoseconds>(now - m_current);
    m_current = now;

    m_dt = std::min(static_cast<f32>(delta.count() * 1e-9), m_fixed_dt);
    m_unscaled_time += m_dt;
    m_time += ( m_dt * m_timescale );

    m_accumulator += m_dt;
    m_timesteps = 0;

    while (m_accumulator >= m_fixed_dt)
    {
      m_accumulator -= m_fixed_dt;
      ++m_timesteps;
      m_fixed_unscaled_time += m_fixed_dt;
      m_fixed_time += ( m_fixed_dt * m_timescale );
    }

    Profiler::Instance().Record("Clock");
  }

  f32 GameClock::DeltaTime() const
  {
    return m_dt * m_timescale;
  }

  f32 GameClock::FixedDeltaTime() const
  {
    return m_fixed_dt * m_timescale;
  }

  f32 GameClock::FixedUnscaledDeltaTime() const
  {
    return m_fixed_dt;
  }

  f32 GameClock::UnscaledDeltaTime() const
  {
    return m_dt;
  }


  f32 GameClock::Time() const
  {
    return m_time;
  }

  f32 GameClock::FixedTime() const
  {
    return m_fixed_time;
  }

  f32 GameClock::FixedUnscaledTime() const
  {
    return m_fixed_unscaled_time;
  }

  f32 GameClock::UnscaledTime() const
  {
    return m_unscaled_time;
  }


  f32 GameClock::FrameCount() const
  {
    return 1.0f / m_dt;
  }

  u32 GameClock::Timesteps() const
  {
    return m_timesteps;
  }
} // namespace DeltaEngine
