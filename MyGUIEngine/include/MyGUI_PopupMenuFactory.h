/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#ifndef __MYGUI_POPUP_MENU_FACTORY_H__
#define __MYGUI_POPUP_MENU_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IWidgetFactory.h"
#include "MyGUI_PopupMenu.h"

namespace MyGUI
{
	namespace factory
	{

		class MYGUI_EXPORT PopupMenuFactory : public BaseWidgetFactory<PopupMenu>
		{
		public:
			PopupMenuFactory();
			~PopupMenuFactory();
		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_POPUP_MENU_FACTORY_H__
