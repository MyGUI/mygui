/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
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
