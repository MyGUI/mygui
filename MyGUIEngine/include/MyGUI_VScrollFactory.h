/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_VSCROLL_FACTORY_H__
#define __MYGUI_VSCROLL_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport VScrollFactory : public WidgetFactoryInterface
		{
		public:
			VScrollFactory();
			~VScrollFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			// методы для парсинга, вызываются для базового класа, поэтому работает и для HScroll
			void Scroll_Range(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Scroll_Position(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Scroll_Page(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Scroll_ViewPage(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_VSCROLL_FACTORY_H__
