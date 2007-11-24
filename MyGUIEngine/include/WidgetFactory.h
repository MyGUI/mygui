#ifndef __WIDGET_FACTORY_H__
#define __WIDGET_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"

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
			WidgetPtr createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// методы для парсинга
			void SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Move(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Size(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Show(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Color(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void FontName(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void FontHeight(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void Alpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void State(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void NeedKey(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void AlignText(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __WIDGET_FACTORY_H__