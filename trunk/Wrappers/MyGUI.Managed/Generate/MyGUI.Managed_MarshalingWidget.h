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



		ref class VScrollBar;
		template <> struct Convert<MyGUI::VScroll*>
		{
			typedef VScrollBar^ Type;
			static VScrollBar^ To(MyGUI::VScroll* _value);
			static MyGUI::VScroll* From(VScrollBar^ _value);
		};
		template <> struct Convert<MyGUI::VScroll*&>
		{
			typedef VScrollBar^% Type;
			static VScrollBar^% To(MyGUI::VScroll*& _value) { throw gcnew System::Exception(); }
			static MyGUI::VScroll*& From(VScrollBar^% _value) { throw gcnew System::Exception(); }
		};



		ref class TabBar;
		template <> struct Convert<MyGUI::Tab*>
		{
			typedef TabBar^ Type;
			static TabBar^ To(MyGUI::Tab* _value);
			static MyGUI::Tab* From(TabBar^ _value);
		};
		template <> struct Convert<MyGUI::Tab*&>
		{
			typedef TabBar^% Type;
			static TabBar^% To(MyGUI::Tab*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Tab*& From(TabBar^% _value) { throw gcnew System::Exception(); }
		};



		ref class StaticText;
		template <> struct Convert<MyGUI::StaticText*>
		{
			typedef StaticText^ Type;
			static StaticText^ To(MyGUI::StaticText* _value);
			static MyGUI::StaticText* From(StaticText^ _value);
		};
		template <> struct Convert<MyGUI::StaticText*&>
		{
			typedef StaticText^% Type;
			static StaticText^% To(MyGUI::StaticText*& _value) { throw gcnew System::Exception(); }
			static MyGUI::StaticText*& From(StaticText^% _value) { throw gcnew System::Exception(); }
		};



		ref class StaticImage;
		template <> struct Convert<MyGUI::StaticImage*>
		{
			typedef StaticImage^ Type;
			static StaticImage^ To(MyGUI::StaticImage* _value);
			static MyGUI::StaticImage* From(StaticImage^ _value);
		};
		template <> struct Convert<MyGUI::StaticImage*&>
		{
			typedef StaticImage^% Type;
			static StaticImage^% To(MyGUI::StaticImage*& _value) { throw gcnew System::Exception(); }
			static MyGUI::StaticImage*& From(StaticImage^% _value) { throw gcnew System::Exception(); }
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
		template <> struct Convert<MyGUI::Progress*>
		{
			typedef ProgressBar^ Type;
			static ProgressBar^ To(MyGUI::Progress* _value);
			static MyGUI::Progress* From(ProgressBar^ _value);
		};
		template <> struct Convert<MyGUI::Progress*&>
		{
			typedef ProgressBar^% Type;
			static ProgressBar^% To(MyGUI::Progress*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Progress*& From(ProgressBar^% _value) { throw gcnew System::Exception(); }
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
		template <> struct Convert<MyGUI::MultiList*>
		{
			typedef MultiListBox^ Type;
			static MultiListBox^ To(MyGUI::MultiList* _value);
			static MyGUI::MultiList* From(MultiListBox^ _value);
		};
		template <> struct Convert<MyGUI::MultiList*&>
		{
			typedef MultiListBox^% Type;
			static MultiListBox^% To(MyGUI::MultiList*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MultiList*& From(MultiListBox^% _value) { throw gcnew System::Exception(); }
		};



		ref class MessageBox;
		template <> struct Convert<MyGUI::Message*>
		{
			typedef MessageBox^ Type;
			static MessageBox^ To(MyGUI::Message* _value);
			static MyGUI::Message* From(MessageBox^ _value);
		};
		template <> struct Convert<MyGUI::Message*&>
		{
			typedef MessageBox^% Type;
			static MessageBox^% To(MyGUI::Message*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Message*& From(MessageBox^% _value) { throw gcnew System::Exception(); }
		};



		ref class MenuCtrl;
		template <> struct Convert<MyGUI::MenuCtrl*>
		{
			typedef MenuCtrl^ Type;
			static MenuCtrl^ To(MyGUI::MenuCtrl* _value);
			static MyGUI::MenuCtrl* From(MenuCtrl^ _value);
		};
		template <> struct Convert<MyGUI::MenuCtrl*&>
		{
			typedef MenuCtrl^% Type;
			static MenuCtrl^% To(MyGUI::MenuCtrl*& _value) { throw gcnew System::Exception(); }
			static MyGUI::MenuCtrl*& From(MenuCtrl^% _value) { throw gcnew System::Exception(); }
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
		template <> struct Convert<MyGUI::List*>
		{
			typedef ListBox^ Type;
			static ListBox^ To(MyGUI::List* _value);
			static MyGUI::List* From(ListBox^ _value);
		};
		template <> struct Convert<MyGUI::List*&>
		{
			typedef ListBox^% Type;
			static ListBox^% To(MyGUI::List*& _value) { throw gcnew System::Exception(); }
			static MyGUI::List*& From(ListBox^% _value) { throw gcnew System::Exception(); }
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



		ref class HScrollBar;
		template <> struct Convert<MyGUI::HScroll*>
		{
			typedef HScrollBar^ Type;
			static HScrollBar^ To(MyGUI::HScroll* _value);
			static MyGUI::HScroll* From(HScrollBar^ _value);
		};
		template <> struct Convert<MyGUI::HScroll*&>
		{
			typedef HScrollBar^% Type;
			static HScrollBar^% To(MyGUI::HScroll*& _value) { throw gcnew System::Exception(); }
			static MyGUI::HScroll*& From(HScrollBar^% _value) { throw gcnew System::Exception(); }
		};



		ref class EditBox;
		template <> struct Convert<MyGUI::Edit*>
		{
			typedef EditBox^ Type;
			static EditBox^ To(MyGUI::Edit* _value);
			static MyGUI::Edit* From(EditBox^ _value);
		};
		template <> struct Convert<MyGUI::Edit*&>
		{
			typedef EditBox^% Type;
			static EditBox^% To(MyGUI::Edit*& _value) { throw gcnew System::Exception(); }
			static MyGUI::Edit*& From(EditBox^% _value) { throw gcnew System::Exception(); }
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
