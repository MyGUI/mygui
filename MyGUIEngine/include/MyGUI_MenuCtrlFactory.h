/*!
	@file
	@author		Albert Semenov
	@date		1/2008
	@module
*/
#ifndef __MYGUI_MENU_CTRL_FACTORY_H__
#define __MYGUI_MENU_CTRL_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_MenuCtrl.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT MenuCtrlFactory : public BaseWidgetFactory<MenuCtrl>
		{
		public:
			MenuCtrlFactory();
			~MenuCtrlFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MENU_CTRL_FACTORY_H__
