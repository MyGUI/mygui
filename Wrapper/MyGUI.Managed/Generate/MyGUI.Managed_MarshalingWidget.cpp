/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "../WidgetHolder.h"

#include "MyGUI.Managed_MarshalingWidget.h"
#include "MyGUI.Managed_IncludeWidgets.h"

namespace MyGUI
{
	namespace Managed
	{

		//InsertPoint

		TabItem^ Convert<MyGUI::TabItem*>::To(MyGUI::TabItem* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (TabItem^)obj->toObject();
			return gcnew TabItem(_value);
		}
		MyGUI::TabItem* Convert<MyGUI::TabItem*>::From(TabItem^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::TabItem>(false);
		}



		MenuItem^ Convert<MyGUI::MenuItem*>::To(MyGUI::MenuItem* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MenuItem^)obj->toObject();
			return gcnew MenuItem(_value);
		}
		MyGUI::MenuItem* Convert<MyGUI::MenuItem*>::From(MenuItem^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MenuItem>(false);
		}



		Window^ Convert<MyGUI::Window*>::To(MyGUI::Window* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (Window^)obj->toObject();
			return gcnew Window(_value);
		}
		MyGUI::Window* Convert<MyGUI::Window*>::From(Window^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Window>(false);
		}



		Widget^ Convert<MyGUI::Widget*>::To(MyGUI::Widget* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (Widget^)obj->toObject();
			return gcnew Widget(_value);
		}
		MyGUI::Widget* Convert<MyGUI::Widget*>::From(Widget^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Widget>(false);
		}



		VScrollBar^ Convert<MyGUI::VScroll*>::To(MyGUI::VScroll* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (VScrollBar^)obj->toObject();
			return gcnew VScrollBar(_value);
		}
		MyGUI::VScroll* Convert<MyGUI::VScroll*>::From(VScrollBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::VScroll>(false);
		}



		TabBar^ Convert<MyGUI::Tab*>::To(MyGUI::Tab* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (TabBar^)obj->toObject();
			return gcnew TabBar(_value);
		}
		MyGUI::Tab* Convert<MyGUI::Tab*>::From(TabBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Tab>(false);
		}



		StaticText^ Convert<MyGUI::StaticText*>::To(MyGUI::StaticText* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (StaticText^)obj->toObject();
			return gcnew StaticText(_value);
		}
		MyGUI::StaticText* Convert<MyGUI::StaticText*>::From(StaticText^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::StaticText>(false);
		}



		StaticImage^ Convert<MyGUI::StaticImage*>::To(MyGUI::StaticImage* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (StaticImage^)obj->toObject();
			return gcnew StaticImage(_value);
		}
		MyGUI::StaticImage* Convert<MyGUI::StaticImage*>::From(StaticImage^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::StaticImage>(false);
		}



		ScrollView^ Convert<MyGUI::ScrollView*>::To(MyGUI::ScrollView* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ScrollView^)obj->toObject();
			return gcnew ScrollView(_value);
		}
		MyGUI::ScrollView* Convert<MyGUI::ScrollView*>::From(ScrollView^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ScrollView>(false);
		}



		RenderBox^ Convert<MyGUI::RenderBox*>::To(MyGUI::RenderBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (RenderBox^)obj->toObject();
			return gcnew RenderBox(_value);
		}
		MyGUI::RenderBox* Convert<MyGUI::RenderBox*>::From(RenderBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::RenderBox>(false);
		}



		ProgressBar^ Convert<MyGUI::Progress*>::To(MyGUI::Progress* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ProgressBar^)obj->toObject();
			return gcnew ProgressBar(_value);
		}
		MyGUI::Progress* Convert<MyGUI::Progress*>::From(ProgressBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Progress>(false);
		}



		PopupMenu^ Convert<MyGUI::PopupMenu*>::To(MyGUI::PopupMenu* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (PopupMenu^)obj->toObject();
			return gcnew PopupMenu(_value);
		}
		MyGUI::PopupMenu* Convert<MyGUI::PopupMenu*>::From(PopupMenu^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::PopupMenu>(false);
		}



		MultiListBox^ Convert<MyGUI::MultiList*>::To(MyGUI::MultiList* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MultiListBox^)obj->toObject();
			return gcnew MultiListBox(_value);
		}
		MyGUI::MultiList* Convert<MyGUI::MultiList*>::From(MultiListBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MultiList>(false);
		}



		MessageBox^ Convert<MyGUI::Message*>::To(MyGUI::Message* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MessageBox^)obj->toObject();
			return gcnew MessageBox(_value);
		}
		MyGUI::Message* Convert<MyGUI::Message*>::From(MessageBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Message>(false);
		}



		MenuCtrl^ Convert<MyGUI::MenuCtrl*>::To(MyGUI::MenuCtrl* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MenuCtrl^)obj->toObject();
			return gcnew MenuCtrl(_value);
		}
		MyGUI::MenuCtrl* Convert<MyGUI::MenuCtrl*>::From(MenuCtrl^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MenuCtrl>(false);
		}



		MenuBar^ Convert<MyGUI::MenuBar*>::To(MyGUI::MenuBar* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MenuBar^)obj->toObject();
			return gcnew MenuBar(_value);
		}
		MyGUI::MenuBar* Convert<MyGUI::MenuBar*>::From(MenuBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MenuBar>(false);
		}



		ListBox^ Convert<MyGUI::List*>::To(MyGUI::List* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ListBox^)obj->toObject();
			return gcnew ListBox(_value);
		}
		MyGUI::List* Convert<MyGUI::List*>::From(ListBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::List>(false);
		}



		ItemBox^ Convert<MyGUI::ItemBox*>::To(MyGUI::ItemBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ItemBox^)obj->toObject();
			return gcnew ItemBox(_value);
		}
		MyGUI::ItemBox* Convert<MyGUI::ItemBox*>::From(ItemBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ItemBox>(false);
		}



		HScrollBar^ Convert<MyGUI::HScroll*>::To(MyGUI::HScroll* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (HScrollBar^)obj->toObject();
			return gcnew HScrollBar(_value);
		}
		MyGUI::HScroll* Convert<MyGUI::HScroll*>::From(HScrollBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::HScroll>(false);
		}



		EditBox^ Convert<MyGUI::Edit*>::To(MyGUI::Edit* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (EditBox^)obj->toObject();
			return gcnew EditBox(_value);
		}
		MyGUI::Edit* Convert<MyGUI::Edit*>::From(EditBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Edit>(false);
		}



		DDContainer^ Convert<MyGUI::DDContainer*>::To(MyGUI::DDContainer* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (DDContainer^)obj->toObject();
			return gcnew DDContainer(_value);
		}
		MyGUI::DDContainer* Convert<MyGUI::DDContainer*>::From(DDContainer^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::DDContainer>(false);
		}



		ComboBox^ Convert<MyGUI::ComboBox*>::To(MyGUI::ComboBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ComboBox^)obj->toObject();
			return gcnew ComboBox(_value);
		}
		MyGUI::ComboBox* Convert<MyGUI::ComboBox*>::From(ComboBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ComboBox>(false);
		}



		Button^ Convert<MyGUI::Button*>::To(MyGUI::Button* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (Button^)obj->toObject();
			return gcnew Button(_value);
		}
		MyGUI::Button* Convert<MyGUI::Button*>::From(Button^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::Button>(false);
		}



	} // namespace Managed
} // namespace MyGUI
