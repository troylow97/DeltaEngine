#include "KeyboardInput.h"
#include <Windows.h>

namespace DeltaEngine
{
void KeyboardInput::Reset()
{
  for (size_t i = 0; i < 256; i ++ )
  {
    if ( m_is_triggered[i] )
    {
      m_is_pressed[i] = true;
      m_is_triggered[i] = false;
    }

    if ( m_is_released[i] )
      m_is_released[i] = false;
  }
}

void KeyboardInput::Update()
{
  // allows us to retrieve the entire sequence of state of each key of the keyboard
  if ( ::GetKeyboardState( m_current_key_state ) )
  {
    // checks the state of each key 
    for ( unsigned int i = 0; i < 256; ++i )
    {
      // bitmasking, only the higher bits of the value are being evaluated 
      // key is pressed if the value is 1 or 2
      if ( m_current_key_state[i] & 0x80 )
      {
        // if triggered
        if ( m_current_key_state[i] != m_previous_key_state[i] )
        {
          m_is_triggered[i] = true;
          m_is_pressed[i] = false;
          m_is_released[i] = false;
        }
        // if pressed
        else
        {
          m_is_triggered[i] = false;
          m_is_pressed[i] = true;
          m_is_released[i] = false;
        }
      }
      // if released
      else
      {
        if ( m_current_key_state[i] != m_previous_key_state[i] )
        {
          m_is_triggered[i] = false;
          m_is_pressed[i] = false;
          m_is_released[i] = true;
        }
        else
        {
          m_is_triggered[i] = false;
          m_is_pressed[i] = false;
          m_is_released[i] = false;
        }
      }
    }
    ::memcpy( m_previous_key_state, m_current_key_state, ( sizeof( unsigned char ) * 256 ) );
  }
}

bool KeyboardInput::IsKeyTriggered( int key )
{
  return m_is_triggered[key];
}

bool KeyboardInput::IsKeyPressed( int key )
{
  return m_is_pressed[key];
}

bool KeyboardInput::IsKeyReleased( int key )
{
  return m_is_released[key];
}

bool KeyboardInput::GetShowLine()
{
  return m_show_line;
}
void KeyboardInput::SetShowLine( bool showLine )
{
  m_show_line = showLine;
}
}