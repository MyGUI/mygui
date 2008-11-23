/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_MenuItemFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		MenuItemFactory::MenuItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("MenuItem_Id") = newDelegate(this, &MenuItemFactory::MenuItem_Id);
			//manager.registerDelegate("MenuItem_Select") = newDelegate(this, &MenuItemFactory::MenuItem_Select);
			//manager.registerDelegate("MenuItem_SmoothSelect") = newDelegate(this, &MenuItemFactory::MenuItem_Select);
		}

		MenuItemFactory::~MenuItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("MenuItem_Id");
			//manager.unregisterDelegate("MenuItem_Select");
			//manager.unregisterDelegate("MenuItem_SmoothSelect");
		}

		void MenuItemFactory::MenuItem_Id(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MenuItemPtr>(_widget)->setItemId(_value);
		}

		/*void MenuItemFactory::MenuItem_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			if (utility::parseBool(_value)) static_cast<MenuItemPtr>(_widget)->setItemSelected();
		}*/

	} // namespace factory
} // namespace MyGUI
