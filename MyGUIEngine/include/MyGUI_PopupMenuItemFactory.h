/*!
	@file
	@author		Albert Semenov
	@date		11/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_ITEM_FACTORY_H__
#define __MYGUI_POPUP_MENU_ITEM_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_PopupMenuItem.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport PopupMenuItemFactory : public BaseWidgetFactory<PopupMenuItem>
		{
		public:
			PopupMenuItemFactory();
			~PopupMenuItemFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_ITEM_FACTORY_H__
