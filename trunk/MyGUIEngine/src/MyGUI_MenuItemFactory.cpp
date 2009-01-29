/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_Precompiled.h"
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
			manager.registerDelegate("MenuItem_Type") = newDelegate(this, &MenuItemFactory::MenuItem_Type);
		}

		MenuItemFactory::~MenuItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("MenuItem_Id");
			manager.unregisterDelegate("MenuItem_Type");
		}

		void MenuItemFactory::MenuItem_Id(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MenuItemPtr>(_widget)->setItemId(_value);
		}

		void MenuItemFactory::MenuItem_Type(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MenuItemPtr>(_widget)->setItemType(MenuItemType::parse(_value));
		}

	} // namespace factory
} // namespace MyGUI
