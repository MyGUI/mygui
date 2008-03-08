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

	class _MyGUIExport UnlinkWidget
	{
	public:
		virtual void _unlinkWidget(WidgetPtr _widget) {}
	};

	typedef std::vector<UnlinkWidget*> VectorUnlinkWidget;

} // namespace MyGUI

#endif // __MYGUI_UNLINK_WIDGET_H__
