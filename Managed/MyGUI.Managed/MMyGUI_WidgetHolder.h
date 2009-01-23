/*!
	@file
	@author		Albert Semenov
	@date
	@module
*/
#pragma once

#include <vcclr.h>

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

} // namespace MMyGUI
