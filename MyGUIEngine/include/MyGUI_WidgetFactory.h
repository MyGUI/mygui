/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_FACTORY_H__
#define __MYGUI_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport WidgetFactory : public WidgetFactoryInterface
		{
		public:
			WidgetFactory();
			~WidgetFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, const Ogre::String& _name);

			// методы для парсинга
			void SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Colour(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_WIDGET_FACTORY_H__