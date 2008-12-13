/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_MENU_ITEM_FACTORY_H__
#define __MYGUI_MENU_ITEM_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_MenuItem.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT MenuItemFactory : public BaseWidgetFactory<MenuItem>
		{
		public:
			MenuItemFactory();
			~MenuItemFactory();

		private:
			void MenuItem_Id(WidgetPtr _widget, const std::string &_key, const std::string &_value);
			void MenuItem_Type(WidgetPtr _widget, const std::string &_key, const std::string &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MENU_ITEM_FACTORY_H__
