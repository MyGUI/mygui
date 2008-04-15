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
			WidgetPtr createWidget(const Ogre::String& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name);

			// методы для парсинга
			void Widget_Caption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_Colour(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_NeedMouse(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_Enabled(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Widget_InheritsAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_WIDGET_FACTORY_H__
