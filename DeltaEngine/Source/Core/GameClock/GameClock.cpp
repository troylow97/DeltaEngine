#include "GameClock.h"

namespace DeltaEngine
{
GameClock::GameClock(f32 fps)
  : m_start(HighResClock::now()),
  m_current(HighResClock::now()),
  m_interval(fps),
  m_elapsed(0.0),
  m_dt(0.0),
  m_g_elapsed(0.0),
  m_g_dt(0.0),
  m_timescale(1.0f),
  m_accumulator(0.0f),
  m_seconds(0.0f),
  m_frame(0),
  m_fps(0),
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

bool GameClock::Update()
{

  TimePoint now = HighResClock::now();
  Nanoseconds delta = std::chrono::duration_cast<Nanoseconds>(now - m_current);
  m_current = now;

  m_dt = static_cast<f32>(delta.count() * 1e-9);
  m_elapsed += m_dt;

  m_accumulator += m_dt;
  m_seconds += m_dt;

  if(m_accumulator >= 1.0f / m_interval )
  {

    if(m_seconds >= 1.0f)
    {
      m_fps = m_frame;
      m_seconds = 0.0f;
      m_frame = 0;
    }

    m_frame++;

    if ( !m_paused )
    {
      m_g_dt = m_accumulator * m_timescale;
      m_g_elapsed += m_g_dt;
    }
    else
      m_g_dt = 0.0f;

    m_accumulator = 0.0f;

    return true;
  }

  return false;
}

f32 GameClock::DeltaTime() const
{
  return m_g_dt;
}

f32 GameClock::RealDeltaTime() const
{
  return m_dt;
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
  return m_fps;
}

} // namespace DeltaEngine