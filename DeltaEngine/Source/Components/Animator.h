#pragma once

#include "Render/AnimationClip.h"
#include "Render/AnimationController.h"

namespace DeltaEngine
{
struct Animator
{
private:

  unsigned int m_Frame;
public:
  float m_Timer;
  float m_Speed;
  std::string m_ControllerKey, m_ClipKey;

  Animator( AnimationController *controller = nullptr );

  unsigned int GetFrame() const;

  void Update(AnimationClip* clip);
};
}
