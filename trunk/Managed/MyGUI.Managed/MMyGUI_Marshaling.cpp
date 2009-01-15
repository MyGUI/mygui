/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "MMyGUI_Marshaling.h"

#include "Generate/MMyGUI_Widget.h"
#include "Generate/MMyGUI_MenuCtrl.h"
#include "Generate/MMyGUI_MenuItem.h"
#include "Generate/MMyGUI_TabItem.h"

namespace MMyGUI
{

	Widget^ ConvertToType<MyGUI::Widget*>::ConvertToValue(MyGUI::Widget* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return obj->toObject(); // ������ ��� �������
		return gcnew Widget(_value); // ���������� ������
	}

	MyGUI::Widget* ConvertToType<MyGUI::Widget*>::ConvertFromValue(Widget^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		return widget;
	}

	MenuItem^ ConvertToType<MyGUI::MenuItem*>::ConvertToValue(MyGUI::MenuItem* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (MenuItem^)obj->toObject(); // ������ ��� �������
		return gcnew MenuItem(_value); // ���������� ������
	}
	MyGUI::MenuItem* ConvertToType<MyGUI::MenuItem*>::ConvertFromValue(MenuItem^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MenuItem>(false);
	}

	MenuCtrl^ ConvertToType<MyGUI::MenuCtrl*>::ConvertToValue(MyGUI::MenuCtrl* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (MenuCtrl^)obj->toObject(); // ������ ��� �������
		return gcnew MenuCtrl(_value); // ���������� ������
	}
	MyGUI::MenuCtrl* ConvertToType<MyGUI::MenuCtrl*>::ConvertFromValue(MenuCtrl^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MenuCtrl>(false);
	}

	TabItem^ ConvertToType<MyGUI::TabItem*>::ConvertToValue(MyGUI::TabItem* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (TabItem^)obj->toObject(); // ������ ��� �������
		return gcnew TabItem(_value); // ���������� ������
	}
	MyGUI::TabItem* ConvertToType<MyGUI::TabItem*>::ConvertFromValue(TabItem^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::TabItem>(false);
	}

}
