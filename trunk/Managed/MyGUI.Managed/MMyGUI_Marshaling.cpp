/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/

#include "MMyGUI_Marshaling.h"
#include "MMyGUI_AllWidgets.h"

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

	StaticImage^ Convert<MyGUI::StaticImage*>::To(MyGUI::StaticImage* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (StaticImage^)obj->toObject(); // ������ ��� �������
		return gcnew StaticImage(_value); // ���������� ������
	}
	MyGUI::StaticImage* Convert<MyGUI::StaticImage*>::From(StaticImage^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::StaticImage>(false);
	}

	Edit^ Convert<MyGUI::Edit*>::To(MyGUI::Edit* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (Edit^)obj->toObject(); // ������ ��� �������
		return gcnew Edit(_value); // ���������� ������
	}
	MyGUI::Edit* Convert<MyGUI::Edit*>::From(Edit^ _value)
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
		if (obj) return (ComboBox^)obj->toObject(); // ������ ��� �������
		return gcnew ComboBox(_value); // ���������� ������
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
		if (obj) return (DDContainer^)obj->toObject(); // ������ ��� �������
		return gcnew DDContainer(_value); // ���������� ������
	}
	MyGUI::DDContainer* Convert<MyGUI::DDContainer*>::From(DDContainer^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::DDContainer>(false);
	}

	VScroll^ Convert<MyGUI::VScroll*>::To(MyGUI::VScroll* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (VScroll^)obj->toObject(); // ������ ��� �������
		return gcnew VScroll(_value); // ���������� ������
	}
	MyGUI::VScroll* Convert<MyGUI::VScroll*>::From(VScroll^ _value)
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
		if (obj) return (ItemBox^)obj->toObject(); // ������ ��� �������
		return gcnew ItemBox(_value); // ���������� ������
	}
	MyGUI::ItemBox* Convert<MyGUI::ItemBox*>::From(ItemBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::ItemBox>(false);
	}

	List^ Convert<MyGUI::List*>::To(MyGUI::List* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (List^)obj->toObject(); // ������ ��� �������
		return gcnew List(_value); // ���������� ������
	}
	MyGUI::List* Convert<MyGUI::List*>::From(List^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::List>(false);
	}

	Message^ Convert<MyGUI::Message*>::To(MyGUI::Message* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (Message^)obj->toObject(); // ������ ��� �������
		return gcnew Message(_value); // ���������� ������
	}
	MyGUI::Message* Convert<MyGUI::Message*>::From(Message^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Message>(false);
	}

	MultiList^ Convert<MyGUI::MultiList*>::To(MyGUI::MultiList* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (MultiList^)obj->toObject(); // ������ ��� �������
		return gcnew MultiList(_value); // ���������� ������
	}
	MyGUI::MultiList* Convert<MyGUI::MultiList*>::From(MultiList^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MultiList>(false);
	}

	Tab^ Convert<MyGUI::Tab*>::To(MyGUI::Tab* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (Tab^)obj->toObject(); // ������ ��� �������
		return gcnew Tab(_value); // ���������� ������
	}
	MyGUI::Tab* Convert<MyGUI::Tab*>::From(Tab^ _value)
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
		if (obj) return (Window^)obj->toObject(); // ������ ��� �������
		return gcnew Window(_value); // ���������� ������
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
		if (obj) return (Canvas^)obj->toObject(); // ������ ��� �������
		return gcnew Canvas(_value); // ���������� ������
	}
	MyGUI::Canvas* Convert<MyGUI::Canvas*>::From(Canvas^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Canvas>(false);
	}

}
