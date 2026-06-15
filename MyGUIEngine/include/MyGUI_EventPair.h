/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_EVENT_PAIR_H_
#define MYGUI_EVENT_PAIR_H_

#include "MyGUI_Prerequest.h"

namespace MyGUI
{
#ifndef MYGUI_DONT_USE_OBSOLETE
	template<typename EventObsolete, typename Event>
	class CompositeEvent
	{
		using IObsoleteDelegate = typename EventObsolete::IDelegate;

	public:
		using IDelegate = typename Event::IDelegate;

		template<class T>
		CompositeEvent& operator=(T* _delegate)
		{
			m_eventObsolete.clear();
			m_event = _delegate;
			return *this;
		}

		template<class T>
		CompositeEvent& operator=(const T& _delegate)
		{
			m_eventObsolete.clear();
			m_event = _delegate;
			return *this;
		}

		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		CompositeEvent& operator=(IObsoleteDelegate* _delegate)
		{
			m_eventObsolete = _delegate;
			m_event.clear();
			return *this;
		}

		template<class T>
		void operator+=(T*) = delete;
		template<class T>
		void operator-=(T*) = delete;

		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator+=(IObsoleteDelegate* _delegate)
		{
			m_eventObsolete += _delegate;
			m_event.clear();
		}

		void operator+=(IDelegate* _delegate)
		{
			m_eventObsolete.clear();
			m_event += _delegate;
		}

		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator-=(IObsoleteDelegate* _delegate)
		{
			m_eventObsolete -= _delegate;
			m_event.clear();
		}

		void operator-=(IDelegate* _delegate)
		{
			m_eventObsolete.clear();
			m_event -= _delegate;
		}

		void clear()
		{
			m_eventObsolete.clear();
			m_event.clear();
		}

		bool empty() const
		{
			return m_eventObsolete.empty() && m_event.empty();
		}

		EventObsolete m_eventObsolete;
		Event m_event;
	};
#endif

	template<typename EventObsolete, typename Event>
#ifdef MYGUI_DONT_USE_OBSOLETE
	using EventPair = Event;
#else
	class EventPair : public CompositeEvent<EventObsolete, Event>
	{
	public:
		using CompositeEvent<EventObsolete, Event>::operator=;
		using CompositeEvent<EventObsolete, Event>::m_eventObsolete;
		using CompositeEvent<EventObsolete, Event>::m_event;

		template<typename TP1>
		void operator()(TP1 p1) const
		{
			m_eventObsolete(p1);
			m_event(p1);
		}

		template<typename TP1, typename TP2>
		void operator()(TP1 p1, TP2 p2) const
		{
			m_eventObsolete(p1, p2);
			m_event(p1, p2);
		}

		template<typename TP1, typename TP2, typename TP3>
		void operator()(TP1 p1, TP2 p2, TP3 p3) const
		{
			m_eventObsolete(p1, p2, p3);
			m_event(p1, p2, p3);
		}

		template<typename TP1, typename TP2, typename TP3, typename TP4>
		void operator()(TP1 p1, TP2 p2, TP3 p3, TP4 p4) const
		{
			m_eventObsolete(p1, p2, p3, p4);
			m_event(p1, p2, p3, p4);
		}

		template<typename TP1, typename TP2, typename TP3, typename TP4, typename TP5>
		void operator()(TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5) const
		{
			m_eventObsolete(p1, p2, p3, p4, p5);
			m_event(p1, p2, p3, p4, p5);
		}

		template<typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6>
		void operator()(TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6) const
		{
			m_eventObsolete(p1, p2, p3, p4, p5, p6);
			m_event(p1, p2, p3, p4, p5, p6);
		}

		template<typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6, typename TP7>
		void operator()(TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7) const
		{
			m_eventObsolete(p1, p2, p3, p4, p5, p6, p7);
			m_event(p1, p2, p3, p4, p5, p6, p7);
		}

		template<
			typename TP1,
			typename TP2,
			typename TP3,
			typename TP4,
			typename TP5,
			typename TP6,
			typename TP7,
			typename TP8>
		void operator()(TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7, TP8 p8) const
		{
			m_eventObsolete(p1, p2, p3, p4, p5, p6, p7, p8);
			m_event(p1, p2, p3, p4, p5, p6, p7, p8);
		}
	};
#endif

	template<typename EventObsolete, typename Event>
#ifdef MYGUI_DONT_USE_OBSOLETE
	using EventPairAddParameter = Event;
#else
	class EventPairAddParameter : public CompositeEvent<EventObsolete, Event>
	{
	public:
		using CompositeEvent<EventObsolete, Event>::operator=;
		using CompositeEvent<EventObsolete, Event>::m_eventObsolete;
		using CompositeEvent<EventObsolete, Event>::m_event;

		// 1 to 2
		template<typename TP1, typename TP2>
		void operator()(TP1 p1, TP2 p2)
		{
			m_eventObsolete(p1);
			m_event(p1, p2);
		}

		// 2 to 3
		template<typename TP1, typename TP2, typename TP3, typename TP4>
		void operator()(TP1 p1, TP2 p2, TP3 p3)
		{
			m_eventObsolete(p1, p2);
			m_event(p1, p2, p3);
		}

		// 3 to 4
		template<typename TP1, typename TP2, typename TP3, typename TP4>
		void operator()(TP1 p1, TP2 p2, TP3 p3, TP4 p4)
		{
			m_eventObsolete(p1, p2, p3);
			m_event(p1, p2, p3, p4);
		}
	};
#endif

	template<typename EventObsolete, typename Event>
#ifdef MYGUI_DONT_USE_OBSOLETE
	using EventPairConvertStringView = Event;
#else
	class EventPairConvertStringView : public CompositeEvent<EventObsolete, Event>
	{
		template<class T>
		T&& convertStringView(T&& value) const noexcept
		{
			return std::forward<T>(value);
		}

		template<class T>
		std::basic_string<T> convertStringView(std::basic_string_view<T> value) const noexcept
		{
			return std::string{value};
		}

	public:
		using CompositeEvent<EventObsolete, Event>::operator=;
		using CompositeEvent<EventObsolete, Event>::m_eventObsolete;
		using CompositeEvent<EventObsolete, Event>::m_event;

		template<typename... Args>
		void operator()(Args&&... args) const
		{
			if (!m_eventObsolete.empty())
				m_eventObsolete(convertStringView(args)...);
			m_event(args...);
		}
	};
#endif

} // namespace MyGUI

#endif // MYGUI_EVENT_PAIR_H_
