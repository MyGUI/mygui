/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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
