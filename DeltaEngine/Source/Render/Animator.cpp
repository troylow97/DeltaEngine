#include "Animator.h"
#include "Core/GlobalStruct.h"
#include "Assets/AssetManager.h"
#include "Core/Debugging/Logger/Log.h"
#include "Core/Math/Math.h"

namespace DeltaEngine
{
Animator::Animator( AnimationController *controller )
  : m_Controller { controller }, m_Clip { m_Controller ? m_Controller->entryAnimation : nullptr },
  m_Timer { 0.0f }, m_Frame { 0 }, m_Speed { 1 }, renderer { nullptr }
{

}

bool Animator::GetBool( std::string paramName )
{
  return parameters[paramName].boolValue;
}

bool Animator::SetBool( std::string paramName, bool value )
{
  if ( parameters[paramName].boolValue == value )
    return parameters.at( paramName ).boolValue;
  parameters[paramName].boolValue = value;
  CheckCondition();
  return parameters[paramName].boolValue;
}

float Animator::GetFloat( std::string paramName )
{
  return parameters[paramName].floatValue;
}

float Animator::SetFloat( std::string paramName, float value )
{
  if ( parameters[paramName].floatValue == value )
    return parameters.at( paramName ).floatValue;
  parameters[paramName].floatValue = value;
  CheckCondition();
  return parameters[paramName].floatValue;
}

unsigned int Animator::GetFrame() const
{
  return m_Frame;
}

void Animator::Update()
{
  if ( !m_Controller )
  {
    return;
  }
  if ( !m_Clip )
  {
    m_Clip = m_Controller->entryAnimation;
  }

  m_Timer += static_cast<float>( DeltaTime() );
  while ( m_Timer > 1.0f * m_Clip->GetTotalFrames() / m_Clip->GetFps() )
    m_Timer -= 1.0f * m_Clip->GetTotalFrames() / m_Clip->GetFps();

  m_Frame = (unsigned int) ( m_Timer * m_Clip->GetFps() );
  m_Frame = Math::Clamp( m_Frame, 0, m_Clip->GetTotalFrames() );

  //renderer->sprite = m_Clip->GetSprite(m_Frame);
}

void Animator::CheckCondition()
{
  AnimationClip *newClip = m_Controller->CheckCondition( m_Clip->GetName(), parameters );
  if ( newClip )
  {
    m_Clip = newClip;
    m_Timer = 0;
  }
}
}
