#ifndef __STATIC_IMAGE_FACTORY_H__
#define __STATIC_IMAGE_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport StaticImageFactory : public WidgetFactoryInterface
		{
		public:
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __STATIC_IMAGE_FACTORY_H__