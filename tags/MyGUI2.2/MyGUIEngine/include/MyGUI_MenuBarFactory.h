/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __MYGUI_MENU_BAR_FACTORY_H__
#define __MYGUI_MENU_BAR_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_MenuBar.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT MenuBarFactory : public BaseWidgetFactory<MenuBar>
		{
		public:
			MenuBarFactory();
			~MenuBarFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MENU_BAR_FACTORY_H__
