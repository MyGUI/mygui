/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_I_WIDGET_FACTORY_H__
#define __MYGUI_I_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Align.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ICroppedRectangle.h"

namespace MyGUI
{

	class _MyGUIExport IWidgetFactory
	{
	public:
        virtual ~IWidgetFactory() { }

		virtual const std::string & getTypeName() = 0;
		virtual WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name) = 0;
	};

	#define MYGUI_RETURN_IS_FALSE_TYPE(type, ptr, key) \
	if (!ptr->isType<type>()) { \
		MYGUI_LOG(Error, "Property '" << key << "' is not supported by '" << ptr->getTypeName() << "' widget"); \
		return; \
	}

} // namespace MyGUI

#endif // __MYGUI_I_WIDGET_FACTORY_H__
