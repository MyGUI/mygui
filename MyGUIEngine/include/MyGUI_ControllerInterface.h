/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#ifndef __MYGUI_CONTROLLER_INTERFACE_H__
#define __MYGUI_CONTROLLER_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class ControllerInterface
	{
	public:
		virtual void _unlinkWidget(WidgetPtr _widget) = 0;
	};

	typedef std::vector<ControllerInterface*> VectorControllerInterface;

} // namespace MyGUI

#endif // __MYGUI_CONTROLLER_INTERFACE_H__