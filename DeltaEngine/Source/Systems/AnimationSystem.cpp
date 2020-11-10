#include "AnimationSystem.h"

namespace DeltaEngine
{
void AnimationSystem::Update()
{
  for ( size_t step = 0; step < env.pClock->Timesteps(); ++step )
    em.ForEach( [&]( EntityID id, Animator &a, State &s, Image &i )
    {
      AnimationController *controller = GetEnv().pManager->Get<AnimationController>( a.m_ControllerKey );
      AnimationClip *newClip = nullptr;
      if ( controller )
      {
        s.parameters.insert( controller->startingParameters.begin(), controller->startingParameters.end() );
        if ( a.m_ClipKey.size() == 0 )
        {
          newClip = controller->entryAnimation;
          a.m_ClipKey = newClip->GetName();
          i.m_Sprite = newClip->GetSprite( a.GetFrame() );
        }
        else
        {
          newClip = controller->CheckCondition( a.m_ClipKey, s.parameters );
          if ( newClip )
          {
            a.m_ClipKey = newClip->GetName();
            a.m_Timer = 0;
          }
          else
          {
            newClip = GetEnv().pManager->Get<AnimationClip>( a.m_ClipKey );

            a.m_Timer += static_cast<float>( FixedDeltaTime() ) * a.m_Speed;
            while ( a.m_Timer > 1.0f * newClip->GetTotalFrames() / newClip->GetFps() )
              a.m_Timer -= 1.0f * newClip->GetTotalFrames() / newClip->GetFps();
          }
          a.Update( newClip );
          i.m_Sprite = newClip->GetSprite( a.GetFrame() );
        }
      }
    } );
}
void AnimationSystem::LateUpdate()
{

}
}

