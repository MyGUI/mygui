/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_SheetFactory.h"
#include "MyGUI_Sheet.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		SheetFactory::SheetFactory()
		{
			// регестрируем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// регестрируем все парсеры
			manager.registerDelegate("Sheet_ButtonWidth") = newDelegate(this, &SheetFactory::Sheet_ButtonWidth);
			manager.registerDelegate("Sheet_Select") = newDelegate(this, &SheetFactory::Sheet_Select);
			manager.registerDelegate("Sheet_SmoothSelect") = newDelegate(this, &SheetFactory::Sheet_Select);
		}

		SheetFactory::~SheetFactory()
		{
			// удаляем себя
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// удаляем все парсеры
			manager.unregisterDelegate("Sheet_ButtonWidth");
			manager.unregisterDelegate("Sheet_Select");
			manager.unregisterDelegate("Sheet_SmoothSelect");
		}

		const Ogre::String& SheetFactory::getType()
		{
			return Sheet::_getType();
		}

		WidgetPtr SheetFactory::createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name)
		{
			return new Sheet(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void SheetFactory::Sheet_ButtonWidth(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(SheetPtr, _widget, _key);
			static_cast<SheetPtr>(_widget)->setSheetButtonWidth(utility::parseInt(_value));
		}

		void SheetFactory::Sheet_Select(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(SheetPtr, _widget, _key);
			if (false == utility::parseBool(_value)) return;
			static_cast<SheetPtr>(_widget)->selectSheet(_key == "Sheet_SmoothSelect");
		}

	} // namespace factory
} // namespace MyGUI
