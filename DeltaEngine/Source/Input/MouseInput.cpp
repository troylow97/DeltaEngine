#include "MouseInput.h"
#include <Windows.h>

namespace DeltaEngine
{
void MouseInput::Reset()
{
    m_move_event = false;

    for (auto& it : m_is_triggered)
    {
        if (it.second)
        {
            m_is_pressed[it.first] = true;
            it.second = false;
        }
    }
    for (auto& it : m_is_released)
    {
        if (it.second)
        {
            it.second = false;
        }
    }
    // scroll event maybe
}

void MouseInput::Update()
{
    POINT _currentPosition = {};
    ::GetCursorPos(&_currentPosition);

    if (m_first_time)
    {
        m_previous_mouse_position = Point(_currentPosition.x, _currentPosition.y);
        m_first_time = false;
    }

    // allows us to retrieve the entire sequence of state of each key of the keyboard
    if (::GetKeyboardState(m_current_key_state))
        {
            // checks the state of each key 
            for (unsigned int i = 0; i < 256; ++i)
            {
                // bitmasking, only the higher bits of the value are being evaluated 
                // key is pressed if the value is 1 or 2
                if (m_current_key_state[i] & 0x80)
                {
                    // if triggered
                    if (m_current_key_state[i] != m_previous_key_state[i])
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
                    if (m_current_key_state[i] != m_previous_key_state[i])
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
            ::memcpy(m_previous_key_state, m_current_key_state, (sizeof(unsigned char) * 256));
        }
    if (_currentPosition.x != m_previous_mouse_position.point_x || _currentPosition.y != m_previous_mouse_position.point_y)
    {
        m_current_mouse_position = Point(m_previous_mouse_position.point_x, m_previous_mouse_position.point_y);
        m_previous_mouse_position = Point(_currentPosition.x, _currentPosition.y);
        m_move_event = true;
    }
}

bool MouseInput::IsMouseTriggered(int key)
{
    return m_is_triggered[key];
}

bool MouseInput::IsMousePressed(int key)
{
    return m_is_pressed[key];
}

bool MouseInput::IsMouseReleased(int key)
{
    return m_is_released[key];
}

bool MouseInput::IsMouseMoved()
{
    return m_move_event;
}

const Point& MouseInput::CurrentPosition()
{
    return m_current_mouse_position;
}

const Point& MouseInput::PreviousPosition()
{
    return m_previous_mouse_position;
}

const Point& MouseInput::CurrentCameraPosition()
{
	return m_current_camera_mouse_position;
}

void MouseInput::SetCurrentPosition(Point _currentPosition)
{
    m_current_mouse_position.point_x = _currentPosition.point_x;
    m_current_mouse_position.point_y = _currentPosition.point_y;
}

void MouseInput::SetPreviousPosition(Point _previousPositon)
{
    m_previous_mouse_position.point_x = _previousPositon.point_x;
    m_previous_mouse_position.point_y = _previousPositon.point_y;
}

void MouseInput::SetCurrentCameraPosition(Point _currentCameraPosition)
{
	m_current_camera_mouse_position.point_x = _currentCameraPosition.point_x;
	m_current_camera_mouse_position.point_y = _currentCameraPosition.point_y;
}

bool MouseInput::EntitySelected()
{
	return m_entity_selected;
}

void MouseInput::SetEntitySelected(bool _setEntitySelected)
{
	m_entity_selected = _setEntitySelected;
}

size_t MouseInput::EntityIDSelected()
{
	return m_entityid_selected;
}

void MouseInput::SetEntityIDSelected(size_t _setEntityIDSelected)
{
	m_entityid_selected = _setEntityIDSelected;
}

}