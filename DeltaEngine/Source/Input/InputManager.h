#pragma once

#include "MouseInput.h"
#include "KeyboardInput.h"
#include "Keys.h"

namespace DeltaEngine
{
class InputManager //: public InputListener
{

  KeyboardInput m_keyboard;
  MouseInput m_mouse;

public:
  InputManager();
  ~InputManager();

  void Reset();
  // to get the input events
  void Update();

  //void addListener(InputListener* listener);
  //void removeListener(InputListener* listener);

  // get static method as InputManager is a singleton class
  // returns a pointer to an only instance of the class
  static InputManager *Get();

  bool IsKeyTriggered( int key );
  bool IsKeyPressed( int key );
  bool IsKeyReleased( int key );
  bool GetShowLine();
  void SetShowLine( bool showLine );

  bool OnMouseMove();

  bool IsMouseTriggered( int key );
  bool IsMousePressed( int key );
  bool IsMouseReleased( int key );
  const Point &CurrentPosition();                              // getter 
  const Point &PreviousPosition();                             // getter
  const Point &CurrentCameraPosition();                        // getter
  void SetCurrentPosition(Point _currentPosition);             // setter
  void SetPreviousPosition(Point _previousPosition);           // setter
  void SetCurrentCameraPosition(Point _currentCameraPosition); // setter
															    
  bool EntitySelected();                                       // getter
  void SetEntitySelected(bool _setEntitySelected);             // setter

  size_t EntityIDSelected();                                   // getter
  void SetEntityIDSelected(size_t _setEntityIDSelected);       // setter

  bool *GetKeys();
};

}