   
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
