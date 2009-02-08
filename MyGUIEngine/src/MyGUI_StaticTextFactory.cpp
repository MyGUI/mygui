/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
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
