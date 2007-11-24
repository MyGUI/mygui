#ifndef __WINDOW_FACTORY_H__
#define __WINDOW_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport WindowFactory : public WidgetFactoryInterface
		{
		public:
			WindowFactory();
			~WindowFactory();

			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			void WindowToStick(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void WindowAutoAlpha(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void WindowMinMax(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __WINDOW_FACTORY_H__