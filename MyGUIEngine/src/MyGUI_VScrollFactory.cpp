/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_VScrollFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		VScrollFactory::VScrollFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("Scroll_Range") = newDelegate(this, &VScrollFactory::Scroll_Range);
			manager.registerDelegate("Scroll_Position") = newDelegate(this, &VScrollFactory::Scroll_Position);
			manager.registerDelegate("Scroll_Page") = newDelegate(this, &VScrollFactory::Scroll_Page);
			manager.registerDelegate("Scroll_ViewPage") = newDelegate(this, &VScrollFactory::Scroll_ViewPage);
			manager.registerDelegate("Scroll_MoveToClick") = newDelegate(this, &VScrollFactory::Scroll_MoveToClick);
		}

		VScrollFactory::~VScrollFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Scroll_Range");
			manager.unregisterDelegate("Scroll_Position");
			manager.unregisterDelegate("Scroll_Page");
			manager.unregisterDelegate("Scroll_ViewPage");
			manager.unregisterDelegate("Scroll_MoveToClick");
		}

		void VScrollFactory::Scroll_Range(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollRange(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollPosition(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_Page(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollPage(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_ViewPage(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setScrollViewPage(utility::parseSizeT(_value));
		}

		void VScrollFactory::Scroll_MoveToClick(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<VScrollPtr>(_widget)->setMoveToClick(utility::parseBool(_value));
		}

	} // namespace factory

} // namespace MyGUI
