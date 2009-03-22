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
