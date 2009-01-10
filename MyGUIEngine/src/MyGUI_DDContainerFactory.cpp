/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_DDContainerFactory.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		DDContainerFactory::DDContainerFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// регестрируем все парсеры
			manager.registerDelegate("DDContainer_NeedDragDrop") = newDelegate(this, &DDContainerFactory::DDContainer_NeedDragDrop);
		}

		DDContainerFactory::~DDContainerFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			// удаляем все парсеры
			manager.unregisterDelegate("DDContainer_NeedDragDrop");
		}

		void DDContainerFactory::DDContainer_NeedDragDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			if (isFalseType(_widget, _key)) return;
			static_cast<DDContainerPtr>(_widget)->setNeedDragDrop(utility::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI
