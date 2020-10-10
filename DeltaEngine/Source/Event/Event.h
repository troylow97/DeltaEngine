#pragma once
#include <sstream>
#include "DE_API.h"
#include "Core/Containers/RingBuffer.h"

namespace DeltaEngine
{
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput =	 BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	//A static function that does run-time checks to see what is the event type
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }
	
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	struct Event
	{
	public:
		bool isHandled = false;
		bool isInitialised = false; //for ring buffer

		virtual EventType GetEventType() const
		{
			return EventType::None;
		}
		virtual const char* GetName() const
		{
			return "NONE";
		}
		virtual int GetCategoryFlags() const
		{
			return 0;
		}
		virtual std::string ToString() const { return GetName(); } // for debugging to add more info when overrided 

		inline bool IsInCategory(EventCategory category){ return GetCategoryFlags() & category;}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event) : 
			m_event(event)
			{}

		template<typename T, typename F>
		bool Dispatch(const F& func)
		{
			//checks to see which event type the current event 
			//that we are currently dispatching is and whether it matches this template argument
			//There is no type-safety to see if its an event
			if (m_event.GetEventType() == T::GetStaticType())
			{
				m_event.isHandled = func(static_cast<T&>(m_event));
				return true;
			}
			return false;
		}
	private:
		Event& m_event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

	class EventManager
	{
	public:
		explicit EventManager() :
			EventQueue{ 1000 }
			{}

		void AddEvent(Event& event)
		{
			EventQueue.Write(event);
		}

		Event& ResolveEvent()
		{
			return EventQueue.Read();
		}

		void GetStatus()
		{
			EventQueue.Printdetails();
		}
		
		inline bool IsEmpty() const
    {
			return EventQueue.Empty();
		}

	private:
		RingBuffer<Event>EventQueue;
		//Each event in the queue has a dispatcher

		//some way to iterate through the queue

		//if event manager is not empty

	};
}