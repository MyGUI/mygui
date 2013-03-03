/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_MenuBar.h"

namespace MyGUI
{

	MenuBar::MenuBar()
	{
		mHideByAccept = false;
		mMenuDropMode = true;
		mIsMenuDrop = false;
		mResizeToContent = false;
	}

} // namespace MyGUI
