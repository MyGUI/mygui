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
#include "MyGUI_StaticTextFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		StaticTextFactory::StaticTextFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();

			// регестрируем все парсеры
			manager.registerDelegate("Text_TextColour") = newDelegate(this, &StaticTextFactory::Text_TextColour);
			manager.registerDelegate("Text_TextAlign") = newDelegate(this, &StaticTextFactory::Text_TextAlign);
			manager.registerDelegate("Text_FontName") = newDelegate(this, &StaticTextFactory::Text_FontName);
			manager.registerDelegate("Text_FontHeight") = newDelegate(this, &StaticTextFactory::Text_FontHeight);
		}

		StaticTextFactory::~StaticTextFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();

			// удаляем все парсеры
			manager.unregisterDelegate("Text_TextColour");
			manager.unregisterDelegate("Text_TextAlign");
			manager.unregisterDelegate("Text_FontName");
			manager.unregisterDelegate("Text_FontHeight");
		}

		void StaticTextFactory::Text_TextColour(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticTextPtr>(_widget)->setTextColour(Colour::parse(_value));
		}

		void StaticTextFactory::Text_TextAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticTextPtr>(_widget)->setTextAlign(Align::parse(_value));
		}

		void StaticTextFactory::Text_FontName(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticTextPtr>(_widget)->setFontName(_value);
		}

		void StaticTextFactory::Text_FontHeight(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<StaticTextPtr>(_widget)->setFontHeight(utility::parseUInt(_value));
		}

	} // namespace factory
} // namespace MyGUI
