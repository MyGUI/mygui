/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_UNLINK_WIDGET_H__
#define __MYGUI_UNLINK_WIDGET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class MYGUI_EXPORT IUnlinkWidget
	{
	public:
		virtual ~IUnlinkWidget() { }
		virtual void _unlinkWidget(Widget* _widget) = 0;
	};

	typedef std::vector<IUnlinkWidget*> VectorIUnlinkWidget;

} // namespace MyGUI

#endif // __MYGUI_UNLINK_WIDGET_H__
