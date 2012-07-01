/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "../WidgetHolder.h"

#include "MyGUI_Managed_MarshalingWidget.h"
#include "MyGUI_Managed_IncludeWidgets.h"

namespace MyGUI
{
	namespace Managed
	{

		//InsertPoint

		MultiListItem^ Convert<MyGUI::MultiListItem*>::To(MyGUI::MultiListItem* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MultiListItem^)obj->toObject();
			return gcnew MultiListItem(_value);
		}
		MyGUI::MultiListItem* Convert<MyGUI::MultiListItem*>::From(MultiListItem^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MultiListItem>(false);
		}
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
		ScrollBar^ Convert<MyGUI::ScrollBar*>::To(MyGUI::ScrollBar* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ScrollBar^)obj->toObject();
			return gcnew ScrollBar(_value);
		}
		MyGUI::ScrollBar* Convert<MyGUI::ScrollBar*>::From(ScrollBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ScrollBar>(false);
		}
		TabControl^ Convert<MyGUI::TabControl*>::To(MyGUI::TabControl* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (TabControl^)obj->toObject();
			return gcnew TabControl(_value);
		}
		MyGUI::TabControl* Convert<MyGUI::TabControl*>::From(TabControl^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::TabControl>(false);
		}
		TextBox^ Convert<MyGUI::TextBox*>::To(MyGUI::TextBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (TextBox^)obj->toObject();
			return gcnew TextBox(_value);
		}
		MyGUI::TextBox* Convert<MyGUI::TextBox*>::From(TextBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::TextBox>(false);
		}
		ImageBox^ Convert<MyGUI::ImageBox*>::To(MyGUI::ImageBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ImageBox^)obj->toObject();
			return gcnew ImageBox(_value);
		}
		MyGUI::ImageBox* Convert<MyGUI::ImageBox*>::From(ImageBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ImageBox>(false);
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
		ProgressBar^ Convert<MyGUI::ProgressBar*>::To(MyGUI::ProgressBar* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ProgressBar^)obj->toObject();
			return gcnew ProgressBar(_value);
		}
		MyGUI::ProgressBar* Convert<MyGUI::ProgressBar*>::From(ProgressBar^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ProgressBar>(false);
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
		MultiListBox^ Convert<MyGUI::MultiListBox*>::To(MyGUI::MultiListBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MultiListBox^)obj->toObject();
			return gcnew MultiListBox(_value);
		}
		MyGUI::MultiListBox* Convert<MyGUI::MultiListBox*>::From(MultiListBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MultiListBox>(false);
		}
		MenuControl^ Convert<MyGUI::MenuControl*>::To(MyGUI::MenuControl* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (MenuControl^)obj->toObject();
			return gcnew MenuControl(_value);
		}
		MyGUI::MenuControl* Convert<MyGUI::MenuControl*>::From(MenuControl^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::MenuControl>(false);
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
		ListBox^ Convert<MyGUI::ListBox*>::To(MyGUI::ListBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (ListBox^)obj->toObject();
			return gcnew ListBox(_value);
		}
		MyGUI::ListBox* Convert<MyGUI::ListBox*>::From(ListBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::ListBox>(false);
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
		EditBox^ Convert<MyGUI::EditBox*>::To(MyGUI::EditBox* _value)
		{
			if (_value == nullptr) return nullptr;
			WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
			if (obj) return (EditBox^)obj->toObject();
			return gcnew EditBox(_value);
		}
		MyGUI::EditBox* Convert<MyGUI::EditBox*>::From(EditBox^ _value)
		{
			if (_value == nullptr) return nullptr;
			MyGUI::Widget* widget = _value->GetNativePtr();
			if (widget == nullptr) return nullptr;
			return widget->castType<MyGUI::EditBox>(false);
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
