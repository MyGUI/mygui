/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_I_SUB_WIDGET_RECT_H__
#define __MYGUI_I_SUB_WIDGET_RECT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISubWidgetRect : public ISubWidget
	{
		MYGUI_RTTI_CHILD_HEADER( ISubWidgetRect, ISubWidget );

	public:
		ISubWidgetRect(const IntCoord & _coord, Align _align, ICroppedRectangle * _parent) :
			ISubWidget(_coord, _align, _parent) { }
		virtual ~ISubWidgetRect() { }

		virtual void _setUVSet(const FloatRect& _rect) { }

		virtual bool firstQueue() { return true; }

	};

} // namespace MyGUI

#endif // __MYGUI_I_SUB_WIDGET_RECT_H__
