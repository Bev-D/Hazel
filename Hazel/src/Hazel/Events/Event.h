#pragma once

#include "Hazel/Debug/Instrumentor.h"
#include "Hazel/Core/Base.h"

#include <functional>

namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

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
		EventCategoryApplication    = BIT(0),
		EventCategoryInput          = BIT(1),
		EventCategoryKeyboard       = BIT(2),
		EventCategoryMouse          = BIT(3),
		EventCategoryMouseButton    = BIT(4)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool Handled = false;

		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

/**
 * @brief 事件派发器类描述
 * EventDispatcher 类旨在将特定类型的事件派发给相应的处理器。
 * 它使用模板方法来实现类型安全的事件处理。
 */
class EventDispatcher
{
public:
    /**
     * @brief 构造函数
     * @param event 事件对象的引用，用于事件派发。
     */
    EventDispatcher(Event& event)
        : m_Event(event)
    {
    }
    
    /**
     * @brief 派发类型为 T 的事件至特定的处理器函数。
     * @tparam T 要派发的事件类型，由上下文推导得出。
     * @tparam F 处理器函数的类型，由上下文推导得出。
     * @param func 处理器函数的引用，当事件类型匹配时将被调用。
     * @return 若事件被派发并处理则返回 true；否则，返回 false。
     * 
     * 此函数首先检查当前事件类型是否与模板参数 T 的类型匹配。
     * 若匹配，则调用处理器函数，若处理器函数返回 true，则标记事件为已处理。
     */
    // F 将由编译器推导
    template<typename T, typename F>
    bool Dispatch(const F& func)
    {
        if (m_Event.GetEventType() == T::GetStaticType())
        {
            m_Event.Handled |= func(static_cast<T&>(m_Event));
            return true;
        }
        return false;
    }

private:
    Event& m_Event; ///< 事件对象的引用，存储需要派发的事件。
};


	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}

}

