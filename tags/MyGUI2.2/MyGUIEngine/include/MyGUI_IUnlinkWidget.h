/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
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
		virtual void _unlinkWidget(WidgetPtr _widget) = 0;
	};

	typedef std::vector<IUnlinkWidget*> VectorIUnlinkWidget;

} // namespace MyGUI

#endif // __MYGUI_UNLINK_WIDGET_H__
