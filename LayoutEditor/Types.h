// пока все виджеты кроме Widget, позже будет со скинами
const int NUM_WIDGETS = 23;

// временно, скоро  будет в отдельно xml файле
struct widget_type{
	std::string widget;
	std::string skin;
}widget_types[NUM_WIDGETS] = {
	"Button","Button",
	"Button","ButtonSmall",
	"Button","CheckBox",
	"ComboBox","ComboBox",
	"Edit", "Edit",
	"Edit", "EditStretch",
	"HScroll","HScroll",
	"List","List",
	"Message","Message",
	"Progress","Progress",
	"RenderBox","RenderBox",
	//"Sheet","Sheet",
	"StaticImage","StaticImage",
	"StaticText","StaticText",
	"Tab","Tab",
	"VScroll","VScroll",
	"Widget","Separator1",
	"Widget","Separator3",
	"Widget","Separator3",
	"Widget","Separator4",
	"Window","Window",
	"Window","WindowC",
	"Window","WindowCS",
	"Window","WindowCSX"
};
// все делегаты, пока еще не знаю, как точно их буду использовать
/*manager.registerDelegate("Widget_Move") = newDelegate(this, &WidgetFactory::Widget_Move);
manager.registerDelegate("Widget_Size") = newDelegate(this, &WidgetFactory::Widget_Size);
manager.registerDelegate("Widget_Show") = newDelegate(this, &WidgetFactory::Widget_Show);
manager.registerDelegate("Widget_Colour") = newDelegate(this, &WidgetFactory::Widget_Colour);
manager.registerDelegate("Widget_FontName") = newDelegate(this, &WidgetFactory::Widget_FontName);
manager.registerDelegate("Widget_FontHeight") = newDelegate(this, &WidgetFactory::Widget_FontHeight);
manager.registerDelegate("Widget_Alpha") = newDelegate(this, &WidgetFactory::Widget_Alpha);
manager.registerDelegate("Widget_State") = newDelegate(this, &WidgetFactory::Widget_State);
manager.registerDelegate("Widget_NeedKey") = newDelegate(this, &WidgetFactory::Widget_NeedKey);
manager.registerDelegate("Widget_AlignText") = newDelegate(this, &WidgetFactory::Widget_AlignText);
manager.registerDelegate("Widget_Caption") = newDelegate(this, &WidgetFactory::Widget_Caption);

manager.registerDelegate("Button_Pressed") = newDelegate(this, &ButtonFactory::Button_Pressed);

manager.registerDelegate("Combo_ModeDrop") = newDelegate(this, &ComboBoxFactory::Combo_ModeDrop);
manager.registerDelegate("Combo_AddString") = newDelegate(this, &ComboBoxFactory::Combo_AddString);

manager.registerDelegate("Edit_CursorPosition") = newDelegate(this, &EditFactory::Edit_CursorPosition);
manager.registerDelegate("Edit_TextSelect") = newDelegate(this, &EditFactory::Edit_TextSelect);
manager.registerDelegate("Edit_ReadOnly") = newDelegate(this, &EditFactory::Edit_ReadOnly);
manager.registerDelegate("Edit_Password") = newDelegate(this, &EditFactory::Edit_Password);
manager.registerDelegate("Edit_MultiLine") = newDelegate(this, &EditFactory::Edit_MultiLine);

manager.registerDelegate("List_AddString") = newDelegate(this, &ListFactory::List_AddString);

manager.registerDelegate("Message_Caption") = newDelegate(this, &MessageFactory::Message_Caption);
manager.registerDelegate("Message_Message") = newDelegate(this, &MessageFactory::Message_Message);
manager.registerDelegate("Message_Modal") = newDelegate(this, &MessageFactory::Message_Modal);
manager.registerDelegate("Message_Button") = newDelegate(this, &MessageFactory::Message_Button);
manager.registerDelegate("Message_AddButton") = newDelegate(this, &MessageFactory::Message_AddButton);
manager.registerDelegate("Message_SmoothShow") = newDelegate(this, &MessageFactory::Message_SmoothShow);
manager.registerDelegate("Message_Fade") = newDelegate(this, &MessageFactory::Message_Fade);

manager.registerDelegate("Progress_Range") = newDelegate(this, &ProgressFactory::Progress_Range);
manager.registerDelegate("Progress_Position") = newDelegate(this, &ProgressFactory::Progress_Position);
manager.registerDelegate("Progress_AutoTrack") = newDelegate(this, &ProgressFactory::Progress_AutoTrack);

manager.registerDelegate("RenderBox_Mesh") = newDelegate(this, &RenderBoxFactory::RenderBox_Mesh);
manager.registerDelegate("RenderBox_AutorotationSpeed") = newDelegate(this, &RenderBoxFactory::RenderBox_AutorotationSpeed);
manager.registerDelegate("RenderBox_BackgroungColour") = newDelegate(this, &RenderBoxFactory::RenderBox_BackgroungColour);
manager.registerDelegate("RenderBox_RotationAngle") = newDelegate(this, &RenderBoxFactory::RenderBox_RotationAngle);
manager.registerDelegate("RenderBox_MouseRotation") = newDelegate(this, &RenderBoxFactory::RenderBox_MouseRotation);

manager.registerDelegate("Sheet_ButtonWidth") = newDelegate(this, &SheetFactory::Sheet_ButtonWidth);
manager.registerDelegate("Sheet_Name") = newDelegate(this, &SheetFactory::Sheet_Name);
manager.registerDelegate("Sheet_Select") = newDelegate(this, &SheetFactory::Sheet_Select);
manager.registerDelegate("Sheet_SmoothSelect") = newDelegate(this, &SheetFactory::Sheet_Select);

manager.registerDelegate("Image_Material") = newDelegate(this, &StaticImageFactory::Image_Material);
manager.registerDelegate("Image_Rect") = newDelegate(this, &StaticImageFactory::Image_Rect);
manager.registerDelegate("Image_Tile") = newDelegate(this, &StaticImageFactory::Image_Tile);
manager.registerDelegate("Image_Num") = newDelegate(this, &StaticImageFactory::Image_Num);

manager.registerDelegate("Tab_ButtonWidth") = newDelegate(this, &TabFactory::Tab_ButtonWidth);
manager.registerDelegate("Tab_ButtonAutoWidth") = newDelegate(this, &TabFactory::Tab_ButtonAutoWidth);
manager.registerDelegate("Tab_SmoothShow") = newDelegate(this, &TabFactory::Tab_SmoothShow);
manager.registerDelegate("Tab_AddSheet") = newDelegate(this, &TabFactory::Tab_AddSheet);
manager.registerDelegate("Tab_SelectSheet") = newDelegate(this, &TabFactory::Tab_SelectSheet);

manager.registerDelegate("Scroll_Range") = newDelegate(this, &VScrollFactory::Scroll_Range);
manager.registerDelegate("Scroll_Position") = newDelegate(this, &VScrollFactory::Scroll_Position);
manager.registerDelegate("Scroll_Page") = newDelegate(this, &VScrollFactory::Scroll_Page);

manager.registerDelegate("Window_AutoAlpha") = newDelegate(this, &WindowFactory::Window_AutoAlpha);
manager.registerDelegate("Window_MinMax") = newDelegate(this, &WindowFactory::Window_MinMax);
manager.registerDelegate("Window_Snap") = newDelegate(this, &WindowFactory::Window_Snap);*/
