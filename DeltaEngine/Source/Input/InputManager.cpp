#include "InputManager.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "Core/Debugging/Profiler/Profiler.h"

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

    Profiler::Instance().Record("Input Manager");
  }

  bool InputManager::IsKeyTriggered(int key)
  {
    return m_keyboard.m_is_triggered[key];
  }

  bool InputManager::IsKeyPressed(int key)
  {
    return m_keyboard.m_is_pressed[key];
  }

  bool InputManager::IsKeyReleased(int key)
  {
    return m_keyboard.m_is_released[key];
  }

  bool InputManager::OnMouseMove()
  {
    return m_mouse.m_move_event;
  }

  bool InputManager::IsMouseTriggered(int key)
  {
    return m_mouse.m_is_triggered[key];
  }

  bool InputManager::IsMousePressed(int key)
  {
    return m_mouse.m_is_pressed[key];
  }

  bool InputManager::IsMouseReleased(int key)
  {
    return m_mouse.m_is_released[key];
  }

  const Point& InputManager::CurrentPosition()
  {
    return m_mouse.m_current_mouse_position;
  }

  const Point& InputManager::PreviousPosition()
  {
    return m_mouse.m_previous_mouse_position;
  }

  bool* InputManager::GetKeys()
  {
    return m_keyboard.m_is_pressed;
  }
}
