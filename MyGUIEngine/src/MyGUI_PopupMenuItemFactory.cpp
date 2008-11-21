/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#include "MyGUI_PopupMenuItemFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		PopupMenuItemFactory::PopupMenuItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			//manager.registerDelegate("PopupMenuItem_ButtonWidth") = newDelegate(this, &PopupMenuItemFactory::PopupMenuItem_ButtonWidth);
			//manager.registerDelegate("PopupMenuItem_Select") = newDelegate(this, &PopupMenuItemFactory::PopupMenuItem_Select);
			//manager.registerDelegate("PopupMenuItem_SmoothSelect") = newDelegate(this, &PopupMenuItemFactory::PopupMenuItem_Select);
		}

		PopupMenuItemFactory::~PopupMenuItemFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			//manager.unregisterDelegate("PopupMenuItem_ButtonWidth");
			//manager.unregisterDelegate("PopupMenuItem_Select");
			//manager.unregisterDelegate("PopupMenuItem_SmoothSelect");
		}

		/*void PopupMenuItemFactory::PopupMenuItem_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<PopupMenuItemPtr>(_widget)->setButtonWidth(utility::parseInt(_value));
		}

		void PopupMenuItemFactory::PopupMenuItem_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			if (utility::parseBool(_value)) static_cast<PopupMenuItemPtr>(_widget)->setItemSelected();
		}*/

	} // namespace factory
} // namespace MyGUI
