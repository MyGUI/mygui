/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
