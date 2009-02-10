   
	ref class IButton;
	template <> struct Convert<MyGUI::Button*>
	{
		typedef IButton^ Type;
		static IButton^ To(MyGUI::Button* _value);
		static MyGUI::Button* From(IButton^ _value);
	};
   
	ref class ICanvas;
	template <> struct Convert<MyGUI::Canvas*>
	{
		typedef ICanvas^ Type;
		static ICanvas^ To(MyGUI::Canvas* _value);
		static MyGUI::Canvas* From(ICanvas^ _value);
	};
   
	ref class IComboBox;
	template <> struct Convert<MyGUI::ComboBox*>
	{
		typedef IComboBox^ Type;
		static IComboBox^ To(MyGUI::ComboBox* _value);
		static MyGUI::ComboBox* From(IComboBox^ _value);
	};
   
	ref class IDDContainer;
	template <> struct Convert<MyGUI::DDContainer*>
	{
		typedef IDDContainer^ Type;
		static IDDContainer^ To(MyGUI::DDContainer* _value);
		static MyGUI::DDContainer* From(IDDContainer^ _value);
	};
   
	ref class IEditBox;
	template <> struct Convert<MyGUI::Edit*>
	{
		typedef IEditBox^ Type;
		static IEditBox^ To(MyGUI::Edit* _value);
		static MyGUI::Edit* From(IEditBox^ _value);
	};
   
	ref class IHScrollBar;
	template <> struct Convert<MyGUI::HScroll*>
	{
		typedef IHScrollBar^ Type;
		static IHScrollBar^ To(MyGUI::HScroll* _value);
		static MyGUI::HScroll* From(IHScrollBar^ _value);
	};
   
	ref class IItemBox;
	template <> struct Convert<MyGUI::ItemBox*>
	{
		typedef IItemBox^ Type;
		static IItemBox^ To(MyGUI::ItemBox* _value);
		static MyGUI::ItemBox* From(IItemBox^ _value);
	};
   
	ref class IListBox;
	template <> struct Convert<MyGUI::List*>
	{
		typedef IListBox^ Type;
		static IListBox^ To(MyGUI::List* _value);
		static MyGUI::List* From(IListBox^ _value);
	};
   
	ref class IMenuBar;
	template <> struct Convert<MyGUI::MenuBar*>
	{
		typedef IMenuBar^ Type;
		static IMenuBar^ To(MyGUI::MenuBar* _value);
		static MyGUI::MenuBar* From(IMenuBar^ _value);
	};
   
	ref class IMenuCtrl;
	template <> struct Convert<MyGUI::MenuCtrl*>
	{
		typedef IMenuCtrl^ Type;
		static IMenuCtrl^ To(MyGUI::MenuCtrl* _value);
		static MyGUI::MenuCtrl* From(IMenuCtrl^ _value);
	};
   
	ref class IMessageBox;
	template <> struct Convert<MyGUI::Message*>
	{
		typedef IMessageBox^ Type;
		static IMessageBox^ To(MyGUI::Message* _value);
		static MyGUI::Message* From(IMessageBox^ _value);
	};
   
	ref class IMultiListBox;
	template <> struct Convert<MyGUI::MultiList*>
	{
		typedef IMultiListBox^ Type;
		static IMultiListBox^ To(MyGUI::MultiList* _value);
		static MyGUI::MultiList* From(IMultiListBox^ _value);
	};
   
	ref class IPopupMenu;
	template <> struct Convert<MyGUI::PopupMenu*>
	{
		typedef IPopupMenu^ Type;
		static IPopupMenu^ To(MyGUI::PopupMenu* _value);
		static MyGUI::PopupMenu* From(IPopupMenu^ _value);
	};
   
	ref class IProgressBar;
	template <> struct Convert<MyGUI::Progress*>
	{
		typedef IProgressBar^ Type;
		static IProgressBar^ To(MyGUI::Progress* _value);
		static MyGUI::Progress* From(IProgressBar^ _value);
	};
   
	ref class IRenderBox;
	template <> struct Convert<MyGUI::RenderBox*>
	{
		typedef IRenderBox^ Type;
		static IRenderBox^ To(MyGUI::RenderBox* _value);
		static MyGUI::RenderBox* From(IRenderBox^ _value);
	};
   
	ref class IScrollView;
	template <> struct Convert<MyGUI::ScrollView*>
	{
		typedef IScrollView^ Type;
		static IScrollView^ To(MyGUI::ScrollView* _value);
		static MyGUI::ScrollView* From(IScrollView^ _value);
	};
   
	ref class IStaticImage;
	template <> struct Convert<MyGUI::StaticImage*>
	{
		typedef IStaticImage^ Type;
		static IStaticImage^ To(MyGUI::StaticImage* _value);
		static MyGUI::StaticImage* From(IStaticImage^ _value);
	};
   
	ref class IStaticText;
	template <> struct Convert<MyGUI::StaticText*>
	{
		typedef IStaticText^ Type;
		static IStaticText^ To(MyGUI::StaticText* _value);
		static MyGUI::StaticText* From(IStaticText^ _value);
	};
   
	ref class ITabBar;
	template <> struct Convert<MyGUI::Tab*>
	{
		typedef ITabBar^ Type;
		static ITabBar^ To(MyGUI::Tab* _value);
		static MyGUI::Tab* From(ITabBar^ _value);
	};
   
	ref class IVScrollBar;
	template <> struct Convert<MyGUI::VScroll*>
	{
		typedef IVScrollBar^ Type;
		static IVScrollBar^ To(MyGUI::VScroll* _value);
		static MyGUI::VScroll* From(IVScrollBar^ _value);
	};
   
	ref class IWidget;
	template <> struct Convert<MyGUI::Widget*>
	{
		typedef IWidget^ Type;
		static IWidget^ To(MyGUI::Widget* _value);
		static MyGUI::Widget* From(IWidget^ _value);
	};
   
	ref class IWindow;
	template <> struct Convert<MyGUI::Window*>
	{
		typedef IWindow^ Type;
		static IWindow^ To(MyGUI::Window* _value);
		static MyGUI::Window* From(IWindow^ _value);
	};
   
	ref class IMenuItem;
	template <> struct Convert<MyGUI::MenuItem*>
	{
		typedef IMenuItem^ Type;
		static IMenuItem^ To(MyGUI::MenuItem* _value);
		static MyGUI::MenuItem* From(IMenuItem^ _value);
	};
   
	ref class ITabItem;
	template <> struct Convert<MyGUI::TabItem*>
	{
		typedef ITabItem^ Type;
		static ITabItem^ To(MyGUI::TabItem* _value);
		static MyGUI::TabItem* From(ITabItem^ _value);
	};
