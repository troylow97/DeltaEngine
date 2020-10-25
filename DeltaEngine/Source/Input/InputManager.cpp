#include "InputManager.h"
#include "KeyboardInput.h"
#include "MouseInput.h"

namespace DeltaEngine
{

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::Reset()
{
  m_keyboard.Reset();
  m_mouse.Reset();
}

void InputManager::Update()
{
  m_keyboard.Update();
  m_mouse.Update();
}

//void InputManager::addListener(InputListener* listener)
//{
//	_listenerSet.insert(listener);
//}
//
//void InputManager::removeListener(InputListener* listener)
//{
//	_listenerSet.erase(listener);
//}

InputManager *InputManager::Get() // might change in the wei lai
{
    // returns the address of the InputManager object
  static InputManager _InputManager;

  return &_InputManager;
}

bool InputManager::IsKeyTriggered( int key )
{
  return m_keyboard.m_is_triggered[key];
}

bool InputManager::IsKeyPressed( int key )
{
  return m_keyboard.m_is_pressed[key];
}

bool InputManager::IsKeyReleased( int key )
{
  return m_keyboard.m_is_released[key];
}

bool InputManager::GetShowLine()
{
  return m_keyboard.m_show_line;
}

void InputManager::SetShowLine( bool showLine )
{
  m_keyboard.m_show_line = showLine;
}

bool InputManager::OnMouseMove()
{
  return m_mouse.m_move_event;
}

bool InputManager::IsMouseTriggered( int key )
{
  return m_mouse.m_is_triggered[key];
}

bool InputManager::IsMousePressed( int key )
{
  return m_mouse.m_is_pressed[key];
}

bool InputManager::IsMouseReleased( int key )
{
  return m_mouse.m_is_released[key];
}

const Point &InputManager::CurrentPosition()
{
  return m_mouse.m_current_mouse_position;
}

const Point &InputManager::PreviousPosition()
{
  return m_mouse.m_previous_mouse_position;
}

const Point& InputManager::CurrentCameraPosition()
{
	return m_mouse.m_current_camera_mouse_position;
}

void InputManager::SetCurrentPosition(Point _currentPosition)
{
    m_mouse.m_current_mouse_position.point_x = _currentPosition.point_x;
    m_mouse.m_current_mouse_position.point_y = _currentPosition.point_y;
}

void InputManager::SetPreviousPosition(Point _previousPositon)
{
    m_mouse.m_previous_mouse_position.point_x = _previousPositon.point_x;
    m_mouse.m_previous_mouse_position.point_y = _previousPositon.point_y;
}

void InputManager::SetCurrentCameraPosition(Point _currentCameraPosition)
{
	m_mouse.m_current_camera_mouse_position.point_x = _currentCameraPosition.point_x;
	m_mouse.m_current_camera_mouse_position.point_y = _currentCameraPosition.point_y;
}

bool InputManager::EntitySelected()
{
	return m_mouse.m_entity_selected;
}

void InputManager::SetEntitySelected(bool _setEntitySelected)
{
	m_mouse.m_entity_selected = _setEntitySelected;
}

size_t InputManager::EntityIDSelected()
{
	return m_mouse.m_entityid_selected;
}

void InputManager::SetEntityIDSelected(size_t _setEntityIDSelected)
{
	m_mouse.m_entityid_selected = _setEntityIDSelected;
}
}