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
			manager.registerDelegate("Combo_ModeDrop") = newDelegate(this, &ComboBoxFactory::Combo_ModeDrop);
			manager.registerDelegate("Combo_AddString") = newDelegate(this, &ComboBoxFactory::Combo_AddString);
		}

		ComboBoxFactory::~ComboBoxFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Combo_ModeDrop");
			manager.unregisterDelegate("Combo_AddString");
		}

		const Ogre::String& ComboBoxFactory::getType()
		{
			return ComboBox::_getType();
		}

		WidgetPtr ComboBoxFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new ComboBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		// методы для парсинга
		void ComboBoxFactory::Combo_ModeDrop(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ComboBoxPtr, _widget, _key);
			static_cast<ComboBoxPtr>(_widget)->setComboModeDrop(utility::parseBool(_value));
		}

		void ComboBoxFactory::Combo_AddString(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(ComboBoxPtr, _widget, _key);
			static_cast<ComboBoxPtr>(_widget)->addItem(_value);
		}

	} // namespace factory
} // namespace MyGUI
