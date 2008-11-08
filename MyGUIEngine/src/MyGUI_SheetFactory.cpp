/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_SheetFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		SheetFactory::SheetFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("Sheet_ButtonWidth") = newDelegate(this, &SheetFactory::Sheet_ButtonWidth);
			manager.registerDelegate("Sheet_Select") = newDelegate(this, &SheetFactory::Sheet_Select);
			//manager.registerDelegate("Sheet_SmoothSelect") = newDelegate(this, &SheetFactory::Sheet_Select);
		}

		SheetFactory::~SheetFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("Sheet_ButtonWidth");
			manager.unregisterDelegate("Sheet_Select");
			//manager.unregisterDelegate("Sheet_SmoothSelect");
		}

		void SheetFactory::Sheet_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<SheetPtr>(_widget)->setButtonWidth(utility::parseInt(_value));
		}

		void SheetFactory::Sheet_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			if (utility::parseBool(_value)) static_cast<SheetPtr>(_widget)->setItemSelected();
		}

	} // namespace factory
} // namespace MyGUI
