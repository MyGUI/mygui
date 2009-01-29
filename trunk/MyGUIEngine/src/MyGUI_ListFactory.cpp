/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_ListFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ListFactory::ListFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("List_AddItem") = newDelegate(this, &ListFactory::List_AddItem);
		}

		ListFactory::~ListFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("List_AddItem");
		}

		void ListFactory::List_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<ListPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
