/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
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
			manager.registerDelegate("Tab_AddItem") = newDelegate(this, &TabFactory::Tab_AddItem);
			manager.registerDelegate("Tab_SelectItem") = newDelegate(this, &TabFactory::Tab_SelectItem);

			// OBSOLETE
			manager.registerDelegate("Tab_AddSheet") = newDelegate(this, &TabFactory::Tab_AddItem);
			// OBSOLETE
			manager.registerDelegate("Tab_SelectSheet") = newDelegate(this, &TabFactory::Tab_SelectItem);
		}

		TabFactory::~TabFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Tab_ButtonWidth");
			manager.unregisterDelegate("Tab_ButtonAutoWidth");
			manager.unregisterDelegate("Tab_SmoothShow");
			manager.unregisterDelegate("Tab_AddItem");
			manager.unregisterDelegate("Tab_SelectItem");

			//OBSOLETE
			manager.unregisterDelegate("Tab_AddSheet");
			//OBSOLETE
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

		void TabFactory::Tab_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->addItem(_value);
		}

		void TabFactory::Tab_SelectItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<TabPtr>(_widget)->setIndexSelected(utility::parseSizeT(_value));
		}

	} // namespace factory

} // namespace MyGUI
