//#pragma once
//
//#include "Math/Point.h"
//
//namespace DeltaEngine::Input
//{
//	// a function callback, input listener is used as an interface (AE doesnt, it takes snapshot of every single key states in a single frame)
//	class DE_API InputListener // interface class
//	{
//	public:
//		virtual ~InputListener() = default;
//
//		// keyboard pure virtual callback functions ---------------
//		virtual void isKeyTriggered(int key) = 0;
//		virtual void isKeyPressed(int key) = 0;
//		virtual void isKeyReleased(int key) = 0;
//
//		// mouse pure virtual callback functions ------------------
//		virtual void onMouseMove(const Point& position) = 0;
//
//		virtual void isLeftMouseTriggered(const Point& position) = 0;
//		virtual void isLeftMousePressed(const Point& position) = 0;
//		virtual void isLeftMouseReleased(const Point& position) = 0;
//
//		virtual void isRightMouseTriggered(const Point& position) = 0;
//		virtual void isRightMousePressed(const Point& position) = 0;
//		virtual void isRightMouseReleased(const Point& position) = 0;
//	};
//}