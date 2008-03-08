/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_FACTORY_INTERFACE_H__
#define __MYGUI_WIDGET_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport WidgetFactoryInterface
	{
	public:
        virtual ~WidgetFactoryInterface() {}

		virtual const Ogre::String& getType() = 0;
		virtual WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent, WidgetCreator * _creator, const Ogre::String& _name) = 0;
	};

	#if MYGUI_DEBUG_MODE == 1
		#define MYGUI_RETURN_IS_FALSE_TYPE(type, ptr, key) \
		if (null == dynamic_cast<type>(ptr)) { \
			MYGUI_LOG(Error, "Property '" << key << "' is not supported by '" << ptr->getWidgetType() << "' widget"); \
			return; \
		}
	#else
		#define MYGUI_RETURN_IS_FALSE_TYPE(type, ptr, key) ((void)0)
	#endif

} // namespace MyGUI

#endif // __MYGUI_WIDGET_FACTORY_INTERFACE_H__
