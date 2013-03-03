/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_I_SUB_WIDGET_RECT_H__
#define __MYGUI_I_SUB_WIDGET_RECT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_Colour.h"

namespace MyGUI
{

	class MYGUI_EXPORT ISubWidgetRect :
		public ISubWidget
	{
		MYGUI_RTTI_DERIVED( ISubWidgetRect )

	public:
		virtual ~ISubWidgetRect() { }

		virtual void _setUVSet(const FloatRect& _rect) { }
		virtual void _setColour(const Colour& _value) { }
	};

} // namespace MyGUI

#endif // __MYGUI_I_SUB_WIDGET_RECT_H__
