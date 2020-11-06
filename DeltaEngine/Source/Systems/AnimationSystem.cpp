#include "AnimationSystem.h"

namespace DeltaEngine
{

void AnimationSystem::Update()
{
  em.ForEach( [&]( EntityID id, Animator &a, Image &s )
  {
    a.Update();
    s.m_Sprite = a.m_Clip->GetSprite( a.GetFrame() );
  } );
}
void AnimationSystem::LateUpdate()
{

}
}

