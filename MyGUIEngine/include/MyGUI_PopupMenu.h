/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_POPUP_MENU_H_
#define MYGUI_POPUP_MENU_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_MenuControl.h"

namespace MyGUI
{

	/** \brief @wpage{PopupMenu}
		PopupMenu widget description should be here.
	*/
	class MYGUI_EXPORT PopupMenu :
		public MenuControl
	{
		MYGUI_RTTI_DERIVED( PopupMenu )

	public:
		PopupMenu();
	};

} // namespace MyGUI

#endif // MYGUI_POPUP_MENU_H_
