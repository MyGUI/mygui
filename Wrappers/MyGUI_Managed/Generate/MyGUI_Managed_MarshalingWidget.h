/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "../Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		//InsertPoint
		ref class MultiListItem;
		template <> struct Convert<MyGUI::MultiListItem*>
		{
			typedef MultiListItem^ Type;
			static MultiListItem^ To(MyGUI::MultiListItem* _value);
			static MyGUI::MultiListItem* From(MultiListItem^ _value);
		};
		template <> struct Convert<MyGUI::MultiListItem*&>
		{
			typedef MultiListItem^% Type;
			static MultiListItem^% To(MyGUI::MultiListItem*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MultiListItem*& From(MultiListItem^% _value) { throw gcnew System::Exception(); }
		};
		ref class TabItem;
		template <> struct Convert<MyGUI::TabItem*>
		{
			typedef TabItem^ Type;
			static TabItem^ To(MyGUI::TabItem* _value);
			static MyGUI::TabItem* From(TabItem^ _value);
		};
		template <> struct Convert<MyGUI::TabItem*&>
		{
			typedef TabItem^% Type;
			static TabItem^% To(MyGUI::TabItem*& _value) { throw gcnew System::Exception(); }
			static MyGUI::TabItem*& From(TabItem^% _value) { throw gcnew System::Exception(); }
		};
		ref class MenuItem;
		template <> struct Convert<MyGUI::MenuItem*>
		{
			typedef MenuItem^ Type;
			static MenuItem^ To(MyGUI::MenuItem* _value);
			static MyGUI::MenuItem* From(MenuItem^ _value);
		};
		template <> struct Convert<MyGUI::MenuItem*&>
		{
			typedef MenuItem^% Type;
			static MenuItem^% To(MyGUI::MenuItem*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MenuItem*& From(MenuItem^% _value) { throw gcnew System::Exception(); }
		};
		ref class Window;
		template <> struct Convert<MyGUI::Window*>
		{
			typedef Window^ Type;
			static Window^ To(MyGUI::Window* _value);
			static MyGUI::Window* From(Window^ _value);
		};
		template <> struct Convert<MyGUI::Window*&>
		{
			typedef Window^% Type;
			static Window^% To(MyGUI::Window*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Window*& From(Window^% _value) { throw gcnew System::Exception(); }
		};
		ref class Widget;
		template <> struct Convert<MyGUI::Widget*>
		{
			typedef Widget^ Type;
			static Widget^ To(MyGUI::Widget* _value);
			static MyGUI::Widget* From(Widget^ _value);
		};
		template <> struct Convert<MyGUI::Widget*&>
		{
			typedef Widget^% Type;
			static Widget^% To(MyGUI::Widget*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Widget*& From(Widget^% _value) { throw gcnew System::Exception(); }
		};
		ref class ScrollBar;
		template <> struct Convert<MyGUI::ScrollBar*>
		{
			typedef ScrollBar^ Type;
			static ScrollBar^ To(MyGUI::ScrollBar* _value);
			static MyGUI::ScrollBar* From(ScrollBar^ _value);
		};
		template <> struct Convert<MyGUI::ScrollBar*&>
		{
			typedef ScrollBar^% Type;
			static ScrollBar^% To(MyGUI::ScrollBar*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ScrollBar*& From(ScrollBar^% _value) { throw gcnew System::Exception(); }
		};
		ref class TabControl;
		template <> struct Convert<MyGUI::TabControl*>
		{
			typedef TabControl^ Type;
			static TabControl^ To(MyGUI::TabControl* _value);
			static MyGUI::TabControl* From(TabControl^ _value);
		};
		template <> struct Convert<MyGUI::TabControl*&>
		{
			typedef TabControl^% Type;
			static TabControl^% To(MyGUI::TabControl*& _value) { throw gcnew System::Exception(); }
			static MyGUI::TabControl*& From(TabControl^% _value) { throw gcnew System::Exception(); }
		};
		ref class TextBox;
		template <> struct Convert<MyGUI::TextBox*>
		{
			typedef TextBox^ Type;
			static TextBox^ To(MyGUI::TextBox* _value);
			static MyGUI::TextBox* From(TextBox^ _value);
		};
		template <> struct Convert<MyGUI::TextBox*&>
		{
			typedef TextBox^% Type;
			static TextBox^% To(MyGUI::TextBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::TextBox*& From(TextBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class ImageBox;
		template <> struct Convert<MyGUI::ImageBox*>
		{
			typedef ImageBox^ Type;
			static ImageBox^ To(MyGUI::ImageBox* _value);
			static MyGUI::ImageBox* From(ImageBox^ _value);
		};
		template <> struct Convert<MyGUI::ImageBox*&>
		{
			typedef ImageBox^% Type;
			static ImageBox^% To(MyGUI::ImageBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ImageBox*& From(ImageBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class ScrollView;
		template <> struct Convert<MyGUI::ScrollView*>
		{
			typedef ScrollView^ Type;
			static ScrollView^ To(MyGUI::ScrollView* _value);
			static MyGUI::ScrollView* From(ScrollView^ _value);
		};
		template <> struct Convert<MyGUI::ScrollView*&>
		{
			typedef ScrollView^% Type;
			static ScrollView^% To(MyGUI::ScrollView*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ScrollView*& From(ScrollView^% _value) { throw gcnew System::Exception(); }
		};
		ref class ProgressBar;
		template <> struct Convert<MyGUI::ProgressBar*>
		{
			typedef ProgressBar^ Type;
			static ProgressBar^ To(MyGUI::ProgressBar* _value);
			static MyGUI::ProgressBar* From(ProgressBar^ _value);
		};
		template <> struct Convert<MyGUI::ProgressBar*&>
		{
			typedef ProgressBar^% Type;
			static ProgressBar^% To(MyGUI::ProgressBar*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ProgressBar*& From(ProgressBar^% _value) { throw gcnew System::Exception(); }
		};
		ref class PopupMenu;
		template <> struct Convert<MyGUI::PopupMenu*>
		{
			typedef PopupMenu^ Type;
			static PopupMenu^ To(MyGUI::PopupMenu* _value);
			static MyGUI::PopupMenu* From(PopupMenu^ _value);
		};
		template <> struct Convert<MyGUI::PopupMenu*&>
		{
			typedef PopupMenu^% Type;
			static PopupMenu^% To(MyGUI::PopupMenu*& _value) { throw gcnew System::Exception(); }
			static MyGUI::PopupMenu*& From(PopupMenu^% _value) { throw gcnew System::Exception(); }
		};
		ref class MultiListBox;
		template <> struct Convert<MyGUI::MultiListBox*>
		{
			typedef MultiListBox^ Type;
			static MultiListBox^ To(MyGUI::MultiListBox* _value);
			static MyGUI::MultiListBox* From(MultiListBox^ _value);
		};
		template <> struct Convert<MyGUI::MultiListBox*&>
		{
			typedef MultiListBox^% Type;
			static MultiListBox^% To(MyGUI::MultiListBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MultiListBox*& From(MultiListBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class MenuControl;
		template <> struct Convert<MyGUI::MenuControl*>
		{
			typedef MenuControl^ Type;
			static MenuControl^ To(MyGUI::MenuControl* _value);
			static MyGUI::MenuControl* From(MenuControl^ _value);
		};
		template <> struct Convert<MyGUI::MenuControl*&>
		{
			typedef MenuControl^% Type;
			static MenuControl^% To(MyGUI::MenuControl*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MenuControl*& From(MenuControl^% _value) { throw gcnew System::Exception(); }
		};
		ref class MenuBar;
		template <> struct Convert<MyGUI::MenuBar*>
		{
			typedef MenuBar^ Type;
			static MenuBar^ To(MyGUI::MenuBar* _value);
			static MyGUI::MenuBar* From(MenuBar^ _value);
		};
		template <> struct Convert<MyGUI::MenuBar*&>
		{
			typedef MenuBar^% Type;
			static MenuBar^% To(MyGUI::MenuBar*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MenuBar*& From(MenuBar^% _value) { throw gcnew System::Exception(); }
		};
		ref class ListBox;
		template <> struct Convert<MyGUI::ListBox*>
		{
			typedef ListBox^ Type;
			static ListBox^ To(MyGUI::ListBox* _value);
			static MyGUI::ListBox* From(ListBox^ _value);
		};
		template <> struct Convert<MyGUI::ListBox*&>
		{
			typedef ListBox^% Type;
			static ListBox^% To(MyGUI::ListBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ListBox*& From(ListBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class ItemBox;
		template <> struct Convert<MyGUI::ItemBox*>
		{
			typedef ItemBox^ Type;
			static ItemBox^ To(MyGUI::ItemBox* _value);
			static MyGUI::ItemBox* From(ItemBox^ _value);
		};
		template <> struct Convert<MyGUI::ItemBox*&>
		{
			typedef ItemBox^% Type;
			static ItemBox^% To(MyGUI::ItemBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ItemBox*& From(ItemBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class EditBox;
		template <> struct Convert<MyGUI::EditBox*>
		{
			typedef EditBox^ Type;
			static EditBox^ To(MyGUI::EditBox* _value);
			static MyGUI::EditBox* From(EditBox^ _value);
		};
		template <> struct Convert<MyGUI::EditBox*&>
		{
			typedef EditBox^% Type;
			static EditBox^% To(MyGUI::EditBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::EditBox*& From(EditBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class DDContainer;
		template <> struct Convert<MyGUI::DDContainer*>
		{
			typedef DDContainer^ Type;
			static DDContainer^ To(MyGUI::DDContainer* _value);
			static MyGUI::DDContainer* From(DDContainer^ _value);
		};
		template <> struct Convert<MyGUI::DDContainer*&>
		{
			typedef DDContainer^% Type;
			static DDContainer^% To(MyGUI::DDContainer*& _value) { throw gcnew System::Exception(); }
			static MyGUI::DDContainer*& From(DDContainer^% _value) { throw gcnew System::Exception(); }
		};
		ref class ComboBox;
		template <> struct Convert<MyGUI::ComboBox*>
		{
			typedef ComboBox^ Type;
			static ComboBox^ To(MyGUI::ComboBox* _value);
			static MyGUI::ComboBox* From(ComboBox^ _value);
		};
		template <> struct Convert<MyGUI::ComboBox*&>
		{
			typedef ComboBox^% Type;
			static ComboBox^% To(MyGUI::ComboBox*& _value) { throw gcnew System::Exception(); }
			static MyGUI::ComboBox*& From(ComboBox^% _value) { throw gcnew System::Exception(); }
		};
		ref class Button;
		template <> struct Convert<MyGUI::Button*>
		{
			typedef Button^ Type;
			static Button^ To(MyGUI::Button* _value);
			static MyGUI::Button* From(Button^ _value);
		};
		template <> struct Convert<MyGUI::Button*&>
		{
			typedef Button^% Type;
			static Button^% To(MyGUI::Button*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Button*& From(Button^% _value) { throw gcnew System::Exception(); }
		};
		
	} // namespace Managed
} // namespace MyGUI
