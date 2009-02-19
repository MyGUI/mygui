   
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
