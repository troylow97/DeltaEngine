#include "AnimationSystem.h"

namespace DeltaEngine
{

void AnimationSystem::Update()
{
  em.ForEach( [&]( EntityID id, Animator &a, SpriteRenderer &s )
  {
    a.Update();
    s.sprite = a.m_Clip->GetSprite( a.GetFrame() );
  } );
}
void AnimationSystem::LateUpdate()
{

}
}

