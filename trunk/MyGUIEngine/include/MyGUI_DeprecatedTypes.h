/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_DEPRECATED_TYPES_H__
#define __MYGUI_DEPRECATED_TYPES_H__

namespace MyGUI
{
	template <typename Type>
	class MemberObsolete
	{
	public:
	};

#ifndef MYGUI_DONT_USE_OBSOLETE

	typedef Widget* WidgetPtr;
	typedef Button* ButtonPtr;
	typedef Window* WindowPtr;
	typedef ComboBox* ComboBoxPtr;
	typedef TabItem* TabItemPtr;
	typedef ItemBox* ItemBoxPtr;
	typedef MenuItem* MenuItemPtr;
	typedef PopupMenu* PopupMenuPtr;
	typedef MenuBar* MenuBarPtr;
	typedef ScrollView* ScrollViewPtr;
	typedef DDContainer* DDContainerPtr;
	typedef Canvas* CanvasPtr;

	class HScroll;
	typedef HScroll* HScrollPtr;

	class VScroll;
	typedef VScroll* VScrollPtr;

	typedef TabItem Sheet;
	typedef TabItem* SheetPtr;

	typedef Canvas RenderBox;
	typedef Canvas* RenderBoxPtr;

	typedef ImageBox StaticImage;
	typedef StaticImage* StaticImagePtr;

	typedef TextBox StaticText;
	typedef StaticText* StaticTextPtr;

	typedef ProgressBar Progress;
	typedef Progress* ProgressPtr;

	typedef ListBox List;
	typedef List* ListPtr;

	typedef EditBox Edit;
	typedef Edit* EditPtr;

	typedef TabControl Tab;
	typedef Tab* TabPtr;

	typedef MultiListBox MultiList;
	typedef MultiList* MultiListPtr;

	typedef MenuControl MenuCtrl;
	typedef MenuCtrl* MenuCtrlPtr;

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI

#endif // __MYGUI_DEPRECATED_TYPES_H__
