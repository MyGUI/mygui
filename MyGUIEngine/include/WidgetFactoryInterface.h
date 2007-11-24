#ifndef __WIDGET_FACTORY_INTERFACE_H__
#define __WIDGET_FACTORY_INTERFACE_H__

#include "Prerequest.h"
#include "WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport WidgetFactoryInterface
	{
	public:
		virtual const Ogre::String& getType() = 0;
		virtual WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name) = 0;
	};

} // namespace MyGUI

#endif // __WIDGET_FACTORY_INTERFACE_H__