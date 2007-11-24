#ifndef __BUTTON_FACTORY_H__
#define __BUTTON_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport ButtonFactory : public WidgetFactoryInterface
		{
		public:
			ButtonFactory::ButtonFactory();
			ButtonFactory::~ButtonFactory();

			// реализация интерфейса фабрики
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// методы для парсинга
			void ButtonPressed(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __BUTTON_FACTORY_H__