   
	IButton^ Convert<MyGUI::Button*>::To(MyGUI::Button* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IButton^)obj->toObject();
		return gcnew IButton(_value);
	}
	MyGUI::Button* Convert<MyGUI::Button*>::From(IButton^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Button>(false);
	}
   
	ICanvas^ Convert<MyGUI::Canvas*>::To(MyGUI::Canvas* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (ICanvas^)obj->toObject();
		return gcnew ICanvas(_value);
	}
	MyGUI::Canvas* Convert<MyGUI::Canvas*>::From(ICanvas^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Canvas>(false);
	}
   
	IComboBox^ Convert<MyGUI::ComboBox*>::To(MyGUI::ComboBox* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IComboBox^)obj->toObject();
		return gcnew IComboBox(_value);
	}
	MyGUI::ComboBox* Convert<MyGUI::ComboBox*>::From(IComboBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::ComboBox>(false);
	}
   
	IDDContainer^ Convert<MyGUI::DDContainer*>::To(MyGUI::DDContainer* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IDDContainer^)obj->toObject();
		return gcnew IDDContainer(_value);
	}
	MyGUI::DDContainer* Convert<MyGUI::DDContainer*>::From(IDDContainer^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::DDContainer>(false);
	}
   
	IEditBox^ Convert<MyGUI::Edit*>::To(MyGUI::Edit* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IEditBox^)obj->toObject();
		return gcnew IEditBox(_value);
	}
	MyGUI::Edit* Convert<MyGUI::Edit*>::From(IEditBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Edit>(false);
	}
   
	IHScrollBar^ Convert<MyGUI::HScroll*>::To(MyGUI::HScroll* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IHScrollBar^)obj->toObject();
		return gcnew IHScrollBar(_value);
	}
	MyGUI::HScroll* Convert<MyGUI::HScroll*>::From(IHScrollBar^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::HScroll>(false);
	}
   
	IItemBox^ Convert<MyGUI::ItemBox*>::To(MyGUI::ItemBox* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IItemBox^)obj->toObject();
		return gcnew IItemBox(_value);
	}
	MyGUI::ItemBox* Convert<MyGUI::ItemBox*>::From(IItemBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::ItemBox>(false);
	}
   
	IListBox^ Convert<MyGUI::List*>::To(MyGUI::List* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IListBox^)obj->toObject();
		return gcnew IListBox(_value);
	}
	MyGUI::List* Convert<MyGUI::List*>::From(IListBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::List>(false);
	}
   
	IMenuBar^ Convert<MyGUI::MenuBar*>::To(MyGUI::MenuBar* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IMenuBar^)obj->toObject();
		return gcnew IMenuBar(_value);
	}
	MyGUI::MenuBar* Convert<MyGUI::MenuBar*>::From(IMenuBar^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MenuBar>(false);
	}
   
	IMenuCtrl^ Convert<MyGUI::MenuCtrl*>::To(MyGUI::MenuCtrl* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IMenuCtrl^)obj->toObject();
		return gcnew IMenuCtrl(_value);
	}
	MyGUI::MenuCtrl* Convert<MyGUI::MenuCtrl*>::From(IMenuCtrl^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MenuCtrl>(false);
	}
   
	IMessageBox^ Convert<MyGUI::Message*>::To(MyGUI::Message* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IMessageBox^)obj->toObject();
		return gcnew IMessageBox(_value);
	}
	MyGUI::Message* Convert<MyGUI::Message*>::From(IMessageBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Message>(false);
	}
   
	IMultiListBox^ Convert<MyGUI::MultiList*>::To(MyGUI::MultiList* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IMultiListBox^)obj->toObject();
		return gcnew IMultiListBox(_value);
	}
	MyGUI::MultiList* Convert<MyGUI::MultiList*>::From(IMultiListBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MultiList>(false);
	}
   
	IPopupMenu^ Convert<MyGUI::PopupMenu*>::To(MyGUI::PopupMenu* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IPopupMenu^)obj->toObject();
		return gcnew IPopupMenu(_value);
	}
	MyGUI::PopupMenu* Convert<MyGUI::PopupMenu*>::From(IPopupMenu^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::PopupMenu>(false);
	}
   
	IProgressBar^ Convert<MyGUI::Progress*>::To(MyGUI::Progress* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IProgressBar^)obj->toObject();
		return gcnew IProgressBar(_value);
	}
	MyGUI::Progress* Convert<MyGUI::Progress*>::From(IProgressBar^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Progress>(false);
	}
   
	IRenderBox^ Convert<MyGUI::RenderBox*>::To(MyGUI::RenderBox* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IRenderBox^)obj->toObject();
		return gcnew IRenderBox(_value);
	}
	MyGUI::RenderBox* Convert<MyGUI::RenderBox*>::From(IRenderBox^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::RenderBox>(false);
	}
   
	IScrollView^ Convert<MyGUI::ScrollView*>::To(MyGUI::ScrollView* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IScrollView^)obj->toObject();
		return gcnew IScrollView(_value);
	}
	MyGUI::ScrollView* Convert<MyGUI::ScrollView*>::From(IScrollView^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::ScrollView>(false);
	}
   
	IStaticImage^ Convert<MyGUI::StaticImage*>::To(MyGUI::StaticImage* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IStaticImage^)obj->toObject();
		return gcnew IStaticImage(_value);
	}
	MyGUI::StaticImage* Convert<MyGUI::StaticImage*>::From(IStaticImage^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::StaticImage>(false);
	}
   
	IStaticText^ Convert<MyGUI::StaticText*>::To(MyGUI::StaticText* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IStaticText^)obj->toObject();
		return gcnew IStaticText(_value);
	}
	MyGUI::StaticText* Convert<MyGUI::StaticText*>::From(IStaticText^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::StaticText>(false);
	}
   
	ITabBar^ Convert<MyGUI::Tab*>::To(MyGUI::Tab* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (ITabBar^)obj->toObject();
		return gcnew ITabBar(_value);
	}
	MyGUI::Tab* Convert<MyGUI::Tab*>::From(ITabBar^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Tab>(false);
	}
   
	IVScrollBar^ Convert<MyGUI::VScroll*>::To(MyGUI::VScroll* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IVScrollBar^)obj->toObject();
		return gcnew IVScrollBar(_value);
	}
	MyGUI::VScroll* Convert<MyGUI::VScroll*>::From(IVScrollBar^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::VScroll>(false);
	}
   
	IWidget^ Convert<MyGUI::Widget*>::To(MyGUI::Widget* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IWidget^)obj->toObject();
		return gcnew IWidget(_value);
	}
	MyGUI::Widget* Convert<MyGUI::Widget*>::From(IWidget^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Widget>(false);
	}
   
	IWindow^ Convert<MyGUI::Window*>::To(MyGUI::Window* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IWindow^)obj->toObject();
		return gcnew IWindow(_value);
	}
	MyGUI::Window* Convert<MyGUI::Window*>::From(IWindow^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::Window>(false);
	}
   
	IMenuItem^ Convert<MyGUI::MenuItem*>::To(MyGUI::MenuItem* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (IMenuItem^)obj->toObject();
		return gcnew IMenuItem(_value);
	}
	MyGUI::MenuItem* Convert<MyGUI::MenuItem*>::From(IMenuItem^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::MenuItem>(false);
	}
   
	ITabItem^ Convert<MyGUI::TabItem*>::To(MyGUI::TabItem* _value)
	{
		if (_value == nullptr) return nullptr;
		WidgetHolder * obj = _value->getUserData< WidgetHolder >(false);
		if (obj) return (ITabItem^)obj->toObject();
		return gcnew ITabItem(_value);
	}
	MyGUI::TabItem* Convert<MyGUI::TabItem*>::From(ITabItem^ _value)
	{
		if (_value == nullptr) return nullptr;
		MyGUI::Widget* widget = _value->GetNativePtr();
		if (widget == nullptr) return nullptr;
		return widget->castType<MyGUI::TabItem>(false);
	}
