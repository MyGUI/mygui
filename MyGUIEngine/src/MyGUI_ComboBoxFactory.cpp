/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#include "MyGUI_ComboBoxFactory.h"
#include "MyGUI_ComboBox.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		ComboBoxFactory::ComboBoxFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("ComboBox_ModeDrop") = newDelegate(this, &ComboBoxFactory::ComboBox_ModeDrop);
			manager.registerDelegate("ComboBox_AddItem") = newDelegate(this, &ComboBoxFactory::ComboBox_AddItem);
		}

		ComboBoxFactory::~ComboBoxFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("ComboBox_ModeDrop");
			manager.unregisterDelegate("ComboBox_AddItem");
		}

		const std::string & ComboBoxFactory::getTypeName()
		{
			return ComboBox::getClassTypeName();
		}

		WidgetPtr ComboBoxFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new ComboBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		// методы для парсинга
		void ComboBoxFactory::ComboBox_ModeDrop(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ComboBox, _widget, _key);
			static_cast<ComboBoxPtr>(_widget)->setComboModeDrop(utility::parseBool(_value));
		}

		void ComboBoxFactory::ComboBox_AddItem(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ComboBox, _widget, _key);
			static_cast<ComboBoxPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
