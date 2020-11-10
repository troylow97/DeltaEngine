#pragma once

#include "Render/AnimationClip.h"

namespace DeltaEngine
{
struct Animator
{
  std::string m_ControllerKey;
  std::string m_ClipKey;

  float m_Timer;
  unsigned m_Frame;
  float m_Speed;

  Animator()
    : m_ControllerKey { "" }, m_ClipKey { "" },
    m_Timer { 0.0f }, m_Frame { 0 }, m_Speed { 1 }
  {

  }

  Animator& operator=(const Animator& rhs)
  {
    m_ControllerKey = std::string( rhs.m_ControllerKey.empty() ? "" : rhs.m_ControllerKey );
    m_ClipKey = std::string( rhs.m_ClipKey.empty() ? "" : rhs.m_ClipKey );
    m_Timer = rhs.m_Timer;
    m_Frame = rhs.m_Frame;
    m_Speed = rhs.m_Speed;
    return *this;
  }
  //Animator( AnimationController *controller );

  unsigned int GetFrame() const;

  void Update(AnimationClip* clip);
};
}
