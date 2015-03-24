/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MENU_BAR_H_
#define MYGUI_MENU_BAR_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_MenuControl.h"

namespace MyGUI
{

	/** \brief @wpage{MenuBar}
		MenuBar widget description should be here.
	*/
	class MYGUI_EXPORT MenuBar :
		public MenuControl
	{
		MYGUI_RTTI_DERIVED( MenuBar )

	public:
		MenuBar();
	};

} // namespace MyGUI

#endif // MYGUI_MENU_BAR_H_
