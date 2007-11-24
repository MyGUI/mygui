#ifndef __VSCROLL_FACTORY_H__
#define __VSCROLL_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport VScrollFactory : public WidgetFactoryInterface
		{
		public:
			VScrollFactory::VScrollFactory();
			VScrollFactory::~VScrollFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// методы для парсинга, вызываются для базового класа, поэтому работает и для HScroll
			void ScrollRange(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ScrollPosition(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ScrollPage(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __VSCROLL_FACTORY_H__