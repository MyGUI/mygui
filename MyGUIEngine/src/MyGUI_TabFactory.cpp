/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_TabFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		TabFactory::TabFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("Tab_ButtonWidth") = newDelegate(this, &TabFactory::Tab_ButtonWidth);
			manager.registerDelegate("Tab_ButtonAutoWidth") = newDelegate(this, &TabFactory::Tab_ButtonAutoWidth);
			manager.registerDelegate("Tab_SmoothShow") = newDelegate(this, &TabFactory::Tab_SmoothShow);
			manager.registerDelegate("Tab_AddSheet") = newDelegate(this, &TabFactory::Tab_AddSheet);
			manager.registerDelegate("Tab_SelectSheet") = newDelegate(this, &TabFactory::Tab_SelectSheet);
		}

		TabFactory::~TabFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Tab_ButtonWidth");
			manager.unregisterDelegate("Tab_ButtonAutoWidth");
			manager.unregisterDelegate("Tab_SmoothShow");
			manager.unregisterDelegate("Tab_AddSheet");
			manager.unregisterDelegate("Tab_SelectSheet");
		}

		void TabFactory::Tab_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->setButtonDefaultWidth(utility::parseInt(_value));
		}

		void TabFactory::Tab_ButtonAutoWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->setButtonAutoWidth(utility::parseBool(_value));
		}

		void TabFactory::Tab_SmoothShow(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->setSmoothShow(utility::parseBool(_value));
		}

		void TabFactory::Tab_AddSheet(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->addItem(_value);
		}

		void TabFactory::Tab_SelectSheet(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->setItemSelectedAt(utility::parseSizeT(_value));
		}

	} // namespace factory

} // namespace MyGUI
