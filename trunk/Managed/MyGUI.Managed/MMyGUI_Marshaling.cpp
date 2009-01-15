/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

//#include "MMyGUI_Align.h"

#include "MMyGUI_Marshaling.h"

#include "Generate/MMyGUI_Widget.h"
#include "Generate/MMyGUI_MenuCtrl.h"
#include "Generate/MMyGUI_MenuItem.h"
#include "Generate/MMyGUI_TabItem.h"


namespace MMyGUI
{

	Widget^ Convert<MyGUI::Widget*>::To(MyGUI::Widget* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return obj->toObject(); // ������ ��� �������
		return gcnew Widget(_value); // ���������� ������
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
		if (obj) return (MenuItem^)obj->toObject(); // ������ ��� �������
		return gcnew MenuItem(_value); // ���������� ������
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
		if (obj) return (MenuCtrl^)obj->toObject(); // ������ ��� �������
		return gcnew MenuCtrl(_value); // ���������� ������
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
		if (obj) return (TabItem^)obj->toObject(); // ������ ��� �������
		return gcnew TabItem(_value); // ���������� ������
	}
	MyGUI::TabItem* Convert<MyGUI::TabItem*>::From(TabItem^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::TabItem>(false);
	}

}
