/*!
	@file
	@author		Albert Semenov
	@date		12/2007
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
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ComboBoxFactory::ComboBoxFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("ComboBox_ModeDrop") = newDelegate(this, &ComboBoxFactory::ComboBox_ModeDrop);
			manager.registerDelegate("ComboBox_AddItem") = newDelegate(this, &ComboBoxFactory::ComboBox_AddItem);
		}

		ComboBoxFactory::~ComboBoxFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("ComboBox_ModeDrop");
			manager.unregisterDelegate("ComboBox_AddItem");
		}

		// методы для парсинга
		void ComboBoxFactory::ComboBox_ModeDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ComboBoxPtr>(_widget)->setComboModeDrop(utility::parseBool(_value));
		}

		void ComboBoxFactory::ComboBox_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ComboBoxPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
