/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_EDIT_FACTORY_H__
#define __MYGUI_EDIT_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport EditFactory : public WidgetFactoryInterface
		{
		public:
			EditFactory();
			~EditFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			// методы для парсинга
			void Edit_CursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_TextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_ReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_Password(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_MultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_PasswordChar(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_MaxTextLength(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_OverflowToTheLeft(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_EDIT_FACTORY_H__
