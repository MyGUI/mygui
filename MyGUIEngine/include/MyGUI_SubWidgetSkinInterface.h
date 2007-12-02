/*!
	@file
	@author		Albert Semenov
	@date		12/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_SKIN_INTERFACE_H__
#define __MYGUI_SUB_WIDGET_SKIN_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	class SubWidgetSkinInterface;
	typedef SubWidgetSkinInterface * SubWidgetSkinPtr;

	class _MyGUIExport SubWidgetSkinInterface : public CroppedRectangleInterface
	{
	public:
		SubWidgetSkinInterface(int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent) :
			CroppedRectangleInterface(_left, _top, _width, _height, _align, _parent)
		{}

		virtual bool _isText() {return false;}

	}; // class _MyGUIExport SubWidgetSkinInterface : public CroppedRectangleInterface

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_SKIN_INTERFACE_H__