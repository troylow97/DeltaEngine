#include "Animator.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"

namespace DeltaEngine
{
  unsigned int Animator::GetFrame() const
  {
    return m_Frame;
  }

  void Animator::Update(AnimationClip* clip)
  {
    m_Frame = static_cast<unsigned>(m_Timer * clip->GetFps());
    m_Frame = Math::Clamp(m_Frame, 0, clip->GetTotalFrames());
  }
}
