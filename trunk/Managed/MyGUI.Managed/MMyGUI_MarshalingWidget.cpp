/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "MMyGUI_MarshalingWidget.h"
#include "MMyGUI_WidgetHolder.h"

#include "Generate/MMyGUI_MarshalingIncludeWidget.h"

/*#include "Generate/MMyGUI_MenuItem.h"
#include "Generate/MMyGUI_TabItem.h"

#include "Generate/MMyGUI_Button.h"
#include "Generate/MMyGUI_Canvas.h"
#include "Generate/MMyGUI_ComboBox.h"
#include "Generate/MMyGUI_DDContainer.h"
#include "Generate/MMyGUI_Edit.h"
#include "Generate/MMyGUI_HScroll.h"
#include "Generate/MMyGUI_ItemBox.h"
#include "Generate/MMyGUI_List.h"
#include "Generate/MMyGUI_MenuBar.h"
#include "Generate/MMyGUI_MenuCtrl.h"
#include "Generate/MMyGUI_Message.h"
#include "Generate/MMyGUI_MultiList.h"
#include "Generate/MMyGUI_PopupMenu.h"
#include "Generate/MMyGUI_Progress.h"
#include "Generate/MMyGUI_RenderBox.h"
#include "Generate/MMyGUI_ScrollView.h"
#include "Generate/MMyGUI_StaticImage.h"
#include "Generate/MMyGUI_StaticText.h"
#include "Generate/MMyGUI_Tab.h"
#include "Generate/MMyGUI_VScroll.h"
#include "Generate/MMyGUI_Widget.h"
#include "Generate/MMyGUI_Window.h"*/

namespace MMyGUI
{

	#include "Generate/MMyGUI_MarshalingWidget.cpp"

	/*Widget^ Convert<MyGUI::Widget*>::To(MyGUI::Widget* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return obj->toObject();
		return gcnew Widget(_value);
	}

	MyGUI::Widget* Convert<MyGUI::Widget*>::From(Widget^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		return widget;
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

	Canvas^ Convert<MyGUI::Canvas*>::To(MyGUI::Canvas* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (Canvas^)obj->toObject();
		return gcnew Canvas(_value);
	}
	MyGUI::Canvas* Convert<MyGUI::Canvas*>::From(Canvas^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Canvas>(false);
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
	}*/

}
