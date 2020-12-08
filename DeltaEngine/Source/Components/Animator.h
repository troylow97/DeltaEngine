/**********************************************************************************
* \file   Animator.h
* \brief  The file contains BLAHBLAHBLAH
* \author Chin, Clara,   X% Code Contribution
* \author Low, Troy,     X% Code Contribution
* \author Ong, Graeme,   X% Code Contribution
* \author Tan, Tong Wee, X% Code Contribution
*
*
* \copyright Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
or disclosure of this file or its contents without the prior
written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#pragma once

#include "Render/AnimationClip.h"

namespace DeltaEngine
{
  struct Animator
  {
    std::string m_ControllerKey;
    std::string m_ClipKey;

    float m_Timer{0.0f};
    float m_Speed{1.0f};

    Animator()
      : m_ControllerKey{""}, m_ClipKey{""},
        m_Timer{0.0f}, m_Speed{1}
    {
    }

    Animator& operator=(const Animator& rhs)
    {
      m_ControllerKey = std::string(rhs.m_ControllerKey);
      m_ClipKey = std::string(rhs.m_ClipKey);
      m_Timer = 0;
      m_Speed = rhs.m_Speed;
      return *this;
    }
  };
}