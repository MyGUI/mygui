/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_EVENT_PAIR_H__
#define __MYGUI_EVENT_PAIR_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	template <typename EventObsolete, typename Event>
	class EventPair
	{
	public:

		template <typename T>
		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator = (T* _delegate)
		{
			m_eventObsolete = _delegate;
			m_event = nullptr;
		}

		MYGUI_OBSOLETE("use : operator += ")
		void operator = (typename Event::IDelegate* _delegate)
		{
			m_eventObsolete = nullptr;
			m_event = _delegate;
		}

		template <typename T>
		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator += (T* _delegate)
		{
			m_eventObsolete += _delegate;
			m_event.clear();
		}

		void operator += (typename Event::IDelegate* _delegate)
		{
			m_eventObsolete.clear();
			m_event += _delegate;
		}

		template <typename T>
		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator -= (T* _delegate)
		{
			m_eventObsolete -= _delegate;
			m_event.clear();
		}

		void operator -= (typename Event::IDelegate* _delegate)
		{
			m_eventObsolete.clear();
			m_event -= _delegate;
		}

		template <typename TP1>
		void operator()( TP1 p1 )
		{
			m_eventObsolete(p1);
			m_event(p1);
		}

		template <typename TP1, typename TP2>
		void operator()( TP1 p1, TP2 p2 )
		{
			m_eventObsolete(p1, p2);
			m_event(p1, p2);
		}

		template <typename TP1, typename TP2, typename TP3>
		void operator()( TP1 p1, TP2 p2, TP3 p3 )
		{
			m_eventObsolete(p1, p2, p3);
			m_event(p1, p2, p3);
		}

		template <typename TP1, typename TP2, typename TP3, typename TP4>
		void operator()( TP1 p1, TP2 p2, TP3 p3, TP4 p4 )
		{
			m_eventObsolete(p1, p2, p3, p4);
			m_event(p1, p2, p3, p4);
		}

		template <typename TP1, typename TP2, typename TP3, typename TP4, typename TP5>
		void operator()( TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5 )
		{
			m_eventObsolete(p1, p2, p3, p4, p5);
			m_event(p1, p2, p3, p4, p5);
		}

		template <typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6>
		void operator()( TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6 )
		{
			m_eventObsolete(p1, p2, p3, p4, p5, p6);
			m_event(p1, p2, p3, p4, p5, p6);
		}

		template <typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6, typename TP7>
		void operator()( TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7 )
		{
			m_eventObsolete(p1, p2, p3, p4, p5, p6, p7);
			m_event(p1, p2, p3, p4, p5, p6, p7);
		}

		template <typename TP1, typename TP2, typename TP3, typename TP4, typename TP5, typename TP6, typename TP7, typename TP8>
		void operator()( TP1 p1, TP2 p2, TP3 p3, TP4 p4, TP5 p5, TP6 p6, TP7 p7, TP8 p8 )
		{
			m_eventObsolete(p1, p2, p3, p4, p5, p6, p7, p8);
			m_event(p1, p2, p3, p4, p5, p6, p7, p8);
		}

		bool empty() const
		{
			return m_eventObsolete.empty() && m_event.empty();
		}

	public:
		EventObsolete m_eventObsolete;
		Event m_event;
	};

	template <typename EventObsolete, typename Event>
	class EventPairAddParameter
	{
	public:

		template <typename T>
		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator = (T* _delegate)
		{
			m_eventObsolete = _delegate;
			m_event = nullptr;
		}

		MYGUI_OBSOLETE("use : operator += ")
		void operator = (typename Event::IDelegate* _delegate)
		{
			m_eventObsolete = nullptr;
			m_event = _delegate;
		}

		template <typename T>
		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator += (T* _delegate)
		{
			m_eventObsolete += _delegate;
			m_event.clear();
		}

		void operator += (typename Event::IDelegate* _delegate)
		{
			m_eventObsolete.clear();
			m_event += _delegate;
		}

		template <typename T>
		MYGUI_OBSOLETE("use : signature : Event::IDelegate * _delegate")
		void operator -= (T* _delegate)
		{
			m_eventObsolete -= _delegate;
			m_event.clear();
		}

		void operator -= (typename Event::IDelegate* _delegate)
		{
			m_eventObsolete.clear();
			m_event -= _delegate;
		}

		// 1 to 2
		template <typename TP1, typename TP2>
		void operator()( TP1 p1, TP2 p2 )
		{
			m_eventObsolete(p1);
			m_event(p1, p2);
		}

		// 2 to 3
		template <typename TP1, typename TP2, typename TP3, typename TP4>
		void operator()( TP1 p1, TP2 p2, TP3 p3 )
		{
			m_eventObsolete(p1, p2);
			m_event(p1, p2, p3);
		}

		// 3 to 4
		template <typename TP1, typename TP2, typename TP3, typename TP4>
		void operator()( TP1 p1, TP2 p2, TP3 p3, TP4 p4 )
		{
			m_eventObsolete(p1, p2, p3);
			m_event(p1, p2, p3, p4);
		}

		bool empty() const
		{
			return m_eventObsolete.empty() && m_event.empty();
		}

	public:
		EventObsolete m_eventObsolete;
		Event m_event;
	};

} // namespace MyGUI

#endif // __MYGUI_EVENT_PAIR_H__
