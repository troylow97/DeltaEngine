/**********************************************************************************
* \file   EngineClock.cpp
* \brief  This file contains the implementation of the engine's clock
*
*         The engine clock is written to support the following:
*         - Timescaling
*         - Timestepping
*         - unscaled / scaled / fixed unscaled / fixed scaled deltaTime 
*         - unscaled / scaled / fixed unscaled / fixed scaled Time tracking
*
* \author Tan, Tong Wee, 100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/

#include "EngineClock.h"

#include "Core/Debugging/Profiler/Profiler.h"
#include "Core/Debugging/Logger/Log.h"

namespace DeltaEngine
{
  EngineClock::EngineClock(f32 fps)
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
      m_frames_tracker(0.0f),
      m_timesteps(0),
      m_framecounts(0),
      m_frames(0)
  {
    DeltaEngine_CORE_INFO("Game clock initialized, FPS Cap - {} ", fps);
  }

  f32 EngineClock::TimeScale() const
  {
    return m_timescale;
  }

  void EngineClock::TimeScale(const f32 scale)
  {
    m_timescale = scale;
  }

  void EngineClock::Update()
  {
    TimePoint now = HighResClock::now();
    Nanoseconds delta = std::chrono::duration_cast<Nanoseconds>(now - m_current);
    m_current = now;

    m_dt = std::min(static_cast<f32>(delta.count() * 1e-9), m_fixed_dt);
    m_unscaled_time += m_dt;
    m_time += (m_dt * m_timescale);

    m_accumulator += m_dt;
    m_timesteps = 0;

    m_frames_tracker += m_dt;
    m_frames++;
    if (m_frames_tracker > 1.0f)
    {
      m_frames_tracker = 0.0f;
      m_framecounts = m_frames;
      m_frames = 0;
    }

    while (m_accumulator >= m_fixed_dt)
    {
      m_accumulator -= m_fixed_dt;
      ++m_timesteps;
      m_fixed_unscaled_time += m_fixed_dt;
      m_fixed_time += (m_fixed_dt * m_timescale);
    }

    Profiler::Instance().Record("Clock");
  }

  f32 EngineClock::DeltaTime() const
  {
    return m_dt * m_timescale;
  }

  f32 EngineClock::FixedDeltaTime() const
  {
    return m_fixed_dt * m_timescale;
  }

  f32 EngineClock::FixedUnscaledDeltaTime() const
  {
    return m_fixed_dt;
  }

  f32 EngineClock::UnscaledDeltaTime() const
  {
    return m_dt;
  }


  f32 EngineClock::Time() const
  {
    return m_time;
  }

  f32 EngineClock::FixedTime() const
  {
    return m_fixed_time;
  }

  f32 EngineClock::FixedUnscaledTime() const
  {
    return m_fixed_unscaled_time;
  }

  f32 EngineClock::UnscaledTime() const
  {
    return m_unscaled_time;
  }


  u32 EngineClock::FrameCount() const
  {
    return m_framecounts;
  }

  u32 EngineClock::Timesteps() const
  {
    return m_timesteps;
  }
} // namespace DeltaEngine
