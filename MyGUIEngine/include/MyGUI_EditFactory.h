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
			EditFactory::EditFactory();
			EditFactory::~EditFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name);

			// методы для парсинга
			void Edit_SetCursorPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetTextSelect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetReadOnly(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetPassword(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Edit_SetMultiLine(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_EDIT_FACTORY_H__