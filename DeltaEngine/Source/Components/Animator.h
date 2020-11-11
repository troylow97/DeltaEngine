#pragma once

#include "Render/AnimationClip.h"

namespace DeltaEngine
{
struct Animator
{
  std::string m_ControllerKey;
  std::string m_ClipKey;

  float m_Timer { 0.0f };
  unsigned m_Frame{0};
  float m_Speed{1.0f};

  Animator()
    : m_ControllerKey { "" }, m_ClipKey { "" },
    m_Timer { 0.0f }, m_Frame { 0 }, m_Speed { 1 }
  {

  }

  Animator& operator=(const Animator& rhs)
  {
    m_ControllerKey = std::string( rhs.m_ControllerKey );
    m_ClipKey = std::string( rhs.m_ClipKey );
    m_Timer = 0;
    m_Frame = 0;
    m_Speed = rhs.m_Speed;
    return *this;
  }

  unsigned int GetFrame() const;

  void Update(AnimationClip* clip);
};
}
