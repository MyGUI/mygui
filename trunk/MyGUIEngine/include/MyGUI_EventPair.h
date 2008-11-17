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
	class EventPair1
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

		template<typename T1>
		void operator()(T1 _value1)
		{
			event1(_value1);
			event2(_value1);
		}

		template<typename T1, typename T2>
		void operator()(T1 _value1, T2 _value2)
		{
			event1(_value1, _value2);
			event2(_value1, _value2);
		}

		template<typename T1, typename T2, typename T3>
		void operator()(T1 _value1, T2 _value2, T3 _value3)
		{
			event1(_value1, _value2, _value3);
			event2(_value1, _value2, _value3);
		}

		template<typename T1, typename T2, typename T3, typename T4>
		void operator()(T1 _value1, T2 _value2, T3 _value3, T4 _value4)
		{
			event1(_value1, _value2, _value3, _value4);
			event2(_value1, _value2, _value3, _value4);
		}

		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		void operator()(T1 _value1, T2 _value2, T3 _value3, T4 _value4, T5 _value5)
		{
			event1(_value1, _value2, _value3, _value4, _value5);
			event2(_value1, _value2, _value3, _value4, _value5);
		}

	private:
		Event1 event1;
		Event2 event2;
	};

	template <typename Event1, typename Event2>
	class EventPair2
	{
	public:

		// для обнуления делегатов и решения неоднозначности
		void operator = (int _null)
		{
			MYGUI_ASSERT(_null == 0, "this operator only for null");
			event1 = null;
			event2 = null;
		}

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
