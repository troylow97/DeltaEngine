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

  //Animator& operator=(const Animator& rhs)
  //{
  //  m_ControllerKey =  rhs.m_ControllerKey ;
  //  m_ClipKey = rhs.m_ClipKey ;
  //  m_Timer = rhs.m_Timer;
  //  m_Frame = rhs.m_Frame;
  //  m_Speed = rhs.m_Speed;
  //  return *this;
  //}

  //Animator( AnimationController *controller );

  unsigned int GetFrame() const;

  void Update(AnimationClip* clip);
};
}
