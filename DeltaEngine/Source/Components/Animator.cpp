#include "Animator.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"

namespace DeltaEngine
{
Animator::Animator( AnimationController *controller )
  : m_ControllerKey { "" }, m_ClipKey { "" },
  m_Timer { 0.0f }, m_Frame { 0 }, m_Speed { 1 }
{

}

unsigned int Animator::GetFrame() const
{
  return m_Frame;
}

void Animator::Update(AnimationClip* clip)
{
  m_Frame = (unsigned int) ( m_Timer * clip->GetFps() );
  m_Frame = Math::Clamp( m_Frame, 0, clip->GetTotalFrames() );
}
}
