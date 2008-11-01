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

	template <typename Event1, typename Event2>
	class EventPair
	{
	public:

		MYGUI_OBSOLETE("use signature : Event2::IDelegate * _delegate")
		void operator = (typename Event1::IDelegate * _delegate)
		{
			event1 = _delegate;
		}

		void operator = (typename Event2::IDelegate * _delegate)
		{
			event2 = _delegate;
		}

		template<typename T1, typename T2>
		void operator()(T1 _value1, T2 _value2)
		{
			event1(_value1);
			event2(_value1, _value2);
		}

	private:
		Event1 event1;
		Event2 event2;
	};

} // namespace MyGUI

#endif // __MYGUI_EVENT_PAIR_H__
