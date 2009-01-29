/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_TabItemFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		// OBSOLETE
		SheetFactory::SheetFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
		}

		TabItemFactory::TabItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);
			// регестрируем все парсеры
			manager.registerDelegate("TabItem_ButtonWidth") = newDelegate(this, &TabItemFactory::TabItem_ButtonWidth);
			manager.registerDelegate("TabItem_Select") = newDelegate(this, &TabItemFactory::TabItem_Select);
			manager.registerDelegate("Sheet_ButtonWidth") = newDelegate(this, &TabItemFactory::TabItem_ButtonWidth);
			manager.registerDelegate("Sheet_Select") = newDelegate(this, &TabItemFactory::TabItem_Select);
			//manager.registerDelegate("TabItem_SmoothSelect") = newDelegate(this, &TabItemFactory::TabItem_Select);
		}

		TabItemFactory::~TabItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("TabItem_ButtonWidth");
			manager.unregisterDelegate("TabItem_Select");
			manager.unregisterDelegate("Sheet_ButtonWidth");
			manager.unregisterDelegate("Sheet_Select");
			//manager.unregisterDelegate("TabItem_SmoothSelect");
		}

		void TabItemFactory::TabItem_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabItemPtr>(_widget)->setButtonWidth(utility::parseInt(_value));
		}

		void TabItemFactory::TabItem_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			if (utility::parseBool(_value)) static_cast<TabItemPtr>(_widget)->setItemSelected();
		}

	} // namespace factory
} // namespace MyGUI
