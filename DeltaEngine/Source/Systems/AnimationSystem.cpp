/**********************************************************************************
* \file   AnimationSystem.cpp
* \brief  The file contains implementation of AnimationSystem.
* \author Ong, Graeme,   100% Code Contribution
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#include "AnimationSystem.h"

#include "Core/Debugging/Profiler/Profiler.h"
#include "Input/InputManager.h"
#include "Input/Keys.h"

namespace DeltaEngine
{
  void AnimationSystem::Update()
  {
    for (size_t step = 0; step < env.pClock->Timesteps(); ++step)
      em.ForEach([&](EntityID id, Animator& a, State& s, Image& i)
      {
        AnimationController* controller = GetEnv().pManager->Get<AnimationController>(a.m_ControllerKey);
        AnimationClip* newClip = nullptr;
        unsigned int frame = 0;
        if (controller)
        {
          s.parameters.insert(controller->startingParameters.begin(), controller->startingParameters.end());
          if (a.m_ClipKey.empty())
          {
            newClip = GetEnv().pManager->Get<AnimationClip>(controller->entryAnimation);

            if (newClip)
            {
              frame = static_cast<unsigned>(a.m_Timer * newClip->GetFps());

              a.m_ClipKey = newClip->GetName();
              i.m_Sprite = newClip->GetSprite(frame);
              a.m_LoopsCompleted = 0;
            }
          }
          else
          {
            newClip = controller->CheckCondition(a.m_ClipKey, s.parameters);
            if (newClip)
            {
              a.m_ClipKey = newClip->GetName();
              a.m_Timer = 0;
              a.m_LoopsCompleted = 0;
            }
            else
            {
              newClip = GetEnv().pManager->Get<AnimationClip>(a.m_ClipKey);

              if (newClip->looping)
              {
                a.m_Timer += static_cast<float>(FixedDeltaTime()) * a.m_Speed;
                while (a.m_Timer > 1.0f * newClip->GetTotalFrames() / newClip->GetFps())
                {
                  a.m_Timer -= 1.0f * newClip->GetTotalFrames() / newClip->GetFps();
                  ++a.m_LoopsCompleted;
                }
              }
              else
              {
                if (a.m_Timer < 1.0f * newClip->GetTotalFrames() / newClip->GetFps())
                  a.m_Timer += static_cast<float>(FixedDeltaTime()) * a.m_Speed;
                else
                  a.m_LoopsCompleted = 1;
              }
            }

            if (newClip)
            {
              frame = static_cast<unsigned>(a.m_Timer * newClip->GetFps());

              Sprite newSprite = newClip->GetSprite(frame);
              if (newSprite)
                i.m_Sprite = newSprite;
            }
          }
        }
        else if (!a.m_ClipKey.empty())
        {
          newClip = GetEnv().pManager->Get<AnimationClip>(a.m_ClipKey);
          if (newClip)
          {
            if (newClip->looping)
            {
              a.m_Timer += static_cast<float>(FixedDeltaTime()) * a.m_Speed;
              while (a.m_Timer > 1.0f * newClip->GetTotalFrames() / newClip->GetFps())
              {
                a.m_Timer -= 1.0f * newClip->GetTotalFrames() / newClip->GetFps();
                ++a.m_LoopsCompleted;
              }
            }
            else
            {
              if (a.m_Timer > 1.0f * newClip->GetTotalFrames() / newClip->GetFps())
                a.m_Timer += static_cast<float>(FixedDeltaTime()) * a.m_Speed;
              else
                a.m_LoopsCompleted = 1;
            }

            frame = static_cast<unsigned>(a.m_Timer * newClip->GetFps());
            Sprite newSprite = newClip->GetSprite(frame);
            if (newSprite)
              i.m_Sprite = newSprite;
          }
        }
      });
    Profiler::Instance().Record("Animation System");
  }

  void AnimationSystem::LateUpdate()
  {
  }
}
