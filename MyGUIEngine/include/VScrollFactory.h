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
			WidgetPtr createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// методы для парсинга
		};

	} // namespace factory
} // namespace MyGUI

#endif // __VSCROLL_FACTORY_H__