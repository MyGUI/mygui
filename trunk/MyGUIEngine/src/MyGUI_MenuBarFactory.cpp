/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#include "MyGUI_MenuBarFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		MenuBarFactory::MenuBarFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("MenuBar_AddItem") = newDelegate(this, &MenuBarFactory::MenuBar_AddItem);
		}

		MenuBarFactory::~MenuBarFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("MenuBar_AddItem");
		}

		void MenuBarFactory::MenuBar_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<MenuBarPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
