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

  //Animator( AnimationController *controller );

  unsigned int GetFrame() const;

  void Update(AnimationClip* clip);
};
}
