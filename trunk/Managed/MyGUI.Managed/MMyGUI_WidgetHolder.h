/*!
	@file
	@author		Albert Semenov
	@date
	@module
*/
#pragma once

#include "MMyGUI_Macros.h"
#include <vcclr.h>

namespace MyGUI
{
	namespace Managed
	{

		ref class MMYGUI_WIDGET_NAME;
		class WidgetHolder
		{
		public:
			WidgetHolder() : object() { }
			WidgetHolder(MMYGUI_WIDGET_NAME ^ _obj) : object(_obj) { }
			~WidgetHolder() { }
			MMYGUI_WIDGET_NAME ^ toObject() { return object; }
		private:
			gcroot<MMYGUI_WIDGET_NAME^> object;
		};

	} // namespace Managed
} // namespace MyGUI
