/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_SHEET_FACTORY_H__
#define __MYGUI_SHEET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport SheetFactory : public WidgetFactoryInterface
		{
		public:
			SheetFactory();
			~SheetFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			void Sheet_ButtonWidth(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Sheet_Select(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_SHEET_FACTORY_H__
