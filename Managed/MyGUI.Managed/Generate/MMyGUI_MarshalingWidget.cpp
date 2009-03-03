   
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
