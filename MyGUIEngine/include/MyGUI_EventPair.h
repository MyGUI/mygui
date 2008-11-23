/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __MYGUI_EVENT_PAIR_H__
#define __MYGUI_EVENT_PAIR_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	template <typename EventObsolete, typename Event>
	class EventPair
	{
	public:

		MYGUI_OBSOLETE("use signature : Event::IDelegate * _delegate")
		void operator = (typename EventObsolete::IDelegate * _delegate)
		{
			m_eventObsolete = _delegate;
			m_event = null;
		}

		void operator = (typename Event::IDelegate * _delegate)
		{
			m_eventObsolete = null;
			m_event = _delegate;
		}

	public:
		EventObsolete m_eventObsolete;
		Event m_event;
	};

} // namespace MyGUI

#endif // __MYGUI_EVENT_PAIR_H__
