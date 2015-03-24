/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_I_SUB_WIDGET_RECT_H_
#define MYGUI_I_SUB_WIDGET_RECT_H_

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

#endif // MYGUI_I_SUB_WIDGET_RECT_H_
