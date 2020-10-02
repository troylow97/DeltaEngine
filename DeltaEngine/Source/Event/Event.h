#pragma once
#include <sstream>
#include "DE_API.h"
//#include "Core/Containers/RingBuffer.h"

namespace DeltaEngine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
		Collision
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput =	 BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryCollision = BIT(5)
	};

	//A static function that does run-time checks to see what is the event type
	#define EVENT_CLASS_TYPE(type)	static EventType GetStaticType() { return EventType::type; }\
																	virtual EventType GetEventType() const override { return GetStaticType(); }\
																	virtual const char* GetName() const override { return #type; }
	
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	struct Event
	{
		bool isHandled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); } // for debugging to add more info when overrided 

		inline bool IsInCategory(EventCategory category){ return GetCategoryFlags() & category;}
	
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& evnt) : 
			_Event(evnt)
			{}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			std::cout << static_cast<unsigned>(_Event.GetEventType()) << std::endl;
			//checks to see which event type the current event 
			//that we are currently dispatching is and whether it matches this template argument
			//There is no type-safety to see if its an event
			if (_Event.GetEventType() == T::GetStaticType())
			{
				_Event.isHandled = func(static_cast<T&>(_Event));
				return true;
			}
			return false;
		}
	private:
		Event& _Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	//class EventManager
	//{
	//public:
	//	explicit EventManager() :
	//		EventQueue{ 1000 }
	//		{}
	//
	//	void addEvent(Event event)
	//	{
	//		EventQueue.write(event);
	//	}
	//
	//	Event& resolveEvent()
	//	{
	//		return EventQueue.read();
	//	}
	//
	//	void getStatus()
	//	{
	//		EventQueue.printdetails();
	//	}
	//	
	//	inline bool isEmpty()
	//	{
	//		return EventQueue.empty();
	//	}
	//
	//private:
	//	RingBuffer<Event>EventQueue;
	//	//Each event in the queue has a dispatcher
	//
	//	//some way to iterate through the queue
	//
	//	//if event manager is not empty
	//
	//
	//
	//};
}