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

	class HScroll;
	class VScroll;

	typedef Widget* WidgetPtr;
	typedef Button* ButtonPtr;
	typedef Window* WindowPtr;
	typedef List* ListPtr;
	typedef HScroll* HScrollPtr;
	typedef VScroll* VScrollPtr;
	typedef Edit* EditPtr;
	typedef ComboBox* ComboBoxPtr;
	typedef StaticText* StaticTextPtr;
	typedef Tab* TabPtr;
	typedef TabItem* TabItemPtr;
	typedef Progress* ProgressPtr;
	typedef ItemBox* ItemBoxPtr;
	typedef MultiList* MultiListPtr;
	typedef Message* MessagePtr;
	typedef MenuCtrl* MenuCtrlPtr;
	typedef MenuItem* MenuItemPtr;
	typedef PopupMenu* PopupMenuPtr;
	typedef MenuBar* MenuBarPtr;
	typedef ScrollView* ScrollViewPtr;
	typedef DDContainer* DDContainerPtr;
	typedef Canvas* CanvasPtr;

	typedef TabItem Sheet;
	typedef TabItem* SheetPtr;
	typedef Canvas RenderBox;
	typedef Canvas* RenderBoxPtr;

	class StaticImage;
	typedef StaticImage* StaticImagePtr;

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI

#endif // __MYGUI_DEPRECATED_TYPES_H__
