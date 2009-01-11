/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_MARSHALING_H__
#define __MMYGUI_MARSHALING_H__

#include <MyGUI.h>

#include "MMyGUI_Utility.h"
#include "MMyGUI_Align.h"
#include "MMyGUI_IntCoord.h"

namespace MMyGUI
{

	ref class Widget;

	class WidgetHolder
	{
	public:
		WidgetHolder() : object() { }
		WidgetHolder(Widget ^ _obj) : object(_obj) { }
		~WidgetHolder() { }
		Widget ^ toObject() { return object; }
	private:
		gcroot<Widget^> object;
	};

	template <typename T> struct ConvertType { typedef T Type; };
	template <> struct ConvertType<MyGUI::Widget*> { typedef Widget^ Type; };

	// базовый шаблон дл€ конверторов переменных
	template <typename T> inline T ConvertValue(T _value) { return _value; }

	// перегрузка дл€ виджетов
	inline Widget^ ConvertValue(MyGUI::Widget* _value)
	{
		if (_value == 0) return nullptr;
		WidgetHolder * sender = _value->getUserData< WidgetHolder >(false);
		if (sender == 0) return nullptr;
		return sender->toObject();
	}

	// перегрузка дл€ строк
	inline System::String^ ConvertValue(const std::string& _value)
	{
		return utf8_to_managed(_value);
	}

}

#endif // __MMYGUI_MARSHALING_H__
