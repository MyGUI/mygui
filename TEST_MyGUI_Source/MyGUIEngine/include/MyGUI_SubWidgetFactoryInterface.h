/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__
#define __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	class _MyGUIExport SubWidgetFactoryInterface
	{
	public:
		virtual const Ogre::String & getType() = 0;
		virtual CroppedRectangleInterface * createSubWidget(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent) = 0;
	};

	template <class ClassName>
	class CroppedRectangleFactory : public SubWidgetFactoryInterface
	{

	public:
		const Ogre::String & getType() {return ClassName::_getType();};

		CroppedRectangleInterface * createSubWidget(const SubWidgetInfo& _info, CroppedRectangleInterface* _parent)
		{
			return new ClassName(_info, _parent);
		}

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__
