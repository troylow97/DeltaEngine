#include "GameClock.h"

namespace DeltaEngine
{
GameClock::GameClock() :
  m_current( HighResClock::now() ),
  m_fps( 0 ),
  m_frame( 0 ),
  m_dt( 0.0f ),
  m_g_dt( 0.0f ),
  m_elapsed( 0.0f ),
  m_g_elapsed( 0.0f ),
  m_timescale( 1.0f ),
  m_accumulator( 0.0f ),
  m_frameDuration( 0.0f ),
  m_interval( 1.0f / 60.0f ),
  m_paused( false )
{}

f32 GameClock::TimeScale() const
{
  return m_timescale;
}

void GameClock::TimeScale( const f32 scale )
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
  const TimePoint now = HighResClock::now();
  const Nanoseconds delta = std::chrono::duration_cast<Nanoseconds>( now - m_current );

  update = false;
  m_current = now;
  m_accumulator += delta.count() * 1e-9;

  if ( m_accumulator >= m_interval )
  {
    update = true;
    m_frameDuration += m_accumulator;
    m_accumulator = 0.0f;
    m_frame++;

    if ( m_frameDuration >= 1.0f )
    {
      m_frameDuration = 0.0f;
      m_fps = m_frame;
      m_frame = 0;
    }

    m_dt = static_cast<float>( delta.count() * 1e-9 );
    m_elapsed += m_dt;

    if ( !m_paused )
    {
      m_g_dt = m_dt * m_timescale;
      m_g_elapsed += m_g_dt;
    }
  }
}

  f32 GameClock::DeltaTime() const
  {
    return m_g_dt;
  }

  f32 GameClock::UnscaledDeltaTime() const
  {
    return m_g_dt / m_timescale;
  }

  f32 GameClock::ElapsedTime() const
  {
    return m_g_elapsed;
  }

  f32 GameClock::UnscaledElapsedTime() const
  {
    return m_elapsed;
  }

  u32 GameClock::FrameRate() const
  {
    return m_fps;
  }

} // namespace DeltaEngine