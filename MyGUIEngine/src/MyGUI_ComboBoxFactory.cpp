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
			manager.registerDelegate("ComboModeDropList") = newDelegate(this, &ComboBoxFactory::setModeDropList);
		}

		ComboBoxFactory::~ComboBoxFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.unregisterDelegate("ComboModeDropList");
		}

		const Ogre::String& ComboBoxFactory::getType()
		{
			return ComboBox::getType();
		}

		WidgetPtr ComboBoxFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name)
		{
			return new ComboBox(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

		// методы для парсинга
		void ComboBoxFactory::setModeDropList(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_TYPE(ComboBoxPtr, _widget);
			static_cast<ComboBoxPtr>(_widget)->setComboModeDrop(util::parseBool(_value));
		}

	} // namespace factory
} // namespace MyGUI