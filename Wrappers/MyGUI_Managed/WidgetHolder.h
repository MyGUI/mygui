/*!
	@file
	@author		Albert Semenov
	@date
	@module
*/
#pragma once

#include <vcclr.h>

namespace MyGUI
{
	namespace Managed
	{

		ref class BaseWidget;
		class WidgetHolder
		{
		public:
			WidgetHolder() : object() { }
			WidgetHolder(BaseWidget ^ _obj) : object(_obj) { }
			~WidgetHolder() { }
			BaseWidget ^ toObject()
			{
				return object;
			}
		private:
			gcroot < BaseWidget ^ > object;
		};

	} // namespace Managed
} // namespace MyGUI
