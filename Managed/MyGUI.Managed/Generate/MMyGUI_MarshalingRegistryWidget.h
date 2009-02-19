   mCreators->Add(gcnew System::String("Button"), gcnew HandleCreator(Button::WidgetCreator));
   mCreators->Add(gcnew System::String("Canvas"), gcnew HandleCreator(Canvas::WidgetCreator));
   mCreators->Add(gcnew System::String("ComboBox"), gcnew HandleCreator(ComboBox::WidgetCreator));
   mCreators->Add(gcnew System::String("DDContainer"), gcnew HandleCreator(DDContainer::WidgetCreator));
   mCreators->Add(gcnew System::String("Edit"), gcnew HandleCreator(EditBox::WidgetCreator));
   mCreators->Add(gcnew System::String("HScroll"), gcnew HandleCreator(HScrollBar::WidgetCreator));
   mCreators->Add(gcnew System::String("ItemBox"), gcnew HandleCreator(ItemBox::WidgetCreator));
