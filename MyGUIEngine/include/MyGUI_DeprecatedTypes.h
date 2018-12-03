/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_DEPRECATED_TYPES_H_
#define MYGUI_DEPRECATED_TYPES_H_

namespace MyGUI
{
	template <typename Type>
	class MemberObsolete
	{
	public:
	};

#ifndef MYGUI_DONT_USE_OBSOLETE

	using WidgetPtr = Widget*;
	using ButtonPtr = Button*;
	using WindowPtr = Window*;
	using ComboBoxPtr = ComboBox*;
	using TabItemPtr = TabItem*;
	using ItemBoxPtr = ItemBox*;
	using MenuItemPtr = MenuItem*;
	using PopupMenuPtr = PopupMenu*;
	using MenuBarPtr = MenuBar*;
	using ScrollViewPtr = ScrollView*;
	using DDContainerPtr = DDContainer*;
	using CanvasPtr = Canvas*;

	class HScroll;
	using HScrollPtr = HScroll*;

	class VScroll;
	using VScrollPtr = VScroll*;

	using Sheet = TabItem;
	using SheetPtr = TabItem*;

	using RenderBox = Canvas;
	using RenderBoxPtr = Canvas*;

	using StaticImage = ImageBox;
	using StaticImagePtr = StaticImage*;

	using StaticText = TextBox;
	using StaticTextPtr = StaticText*;

	using Progress = ProgressBar;
	using ProgressPtr = Progress*;

	using List = ListBox;
	using ListPtr = List*;

	using Edit = EditBox;
	using EditPtr = Edit*;

	using Tab = TabControl;
	using TabPtr = Tab*;

	using MultiList = MultiListBox;
	using MultiListPtr = MultiList*;

	using MenuCtrl = MenuControl;
	using MenuCtrlPtr = MenuCtrl*;

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI

#endif // MYGUI_DEPRECATED_TYPES_H_
