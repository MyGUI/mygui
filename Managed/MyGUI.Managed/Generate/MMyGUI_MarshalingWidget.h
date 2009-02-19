   
	ref class Button;
	template <> struct Convert<MyGUI::Button*>
	{
		typedef Button^ Type;
		static Button^ To(MyGUI::Button* _value);
		static MyGUI::Button* From(Button^ _value);
	};
   
	ref class Canvas;
	template <> struct Convert<MyGUI::Canvas*>
	{
		typedef Canvas^ Type;
		static Canvas^ To(MyGUI::Canvas* _value);
		static MyGUI::Canvas* From(Canvas^ _value);
	};
   
	ref class ComboBox;
	template <> struct Convert<MyGUI::ComboBox*>
	{
		typedef ComboBox^ Type;
		static ComboBox^ To(MyGUI::ComboBox* _value);
		static MyGUI::ComboBox* From(ComboBox^ _value);
	};
   
	ref class DDContainer;
	template <> struct Convert<MyGUI::DDContainer*>
	{
		typedef DDContainer^ Type;
		static DDContainer^ To(MyGUI::DDContainer* _value);
		static MyGUI::DDContainer* From(DDContainer^ _value);
	};
   
	ref class EditBox;
	template <> struct Convert<MyGUI::Edit*>
	{
		typedef EditBox^ Type;
		static EditBox^ To(MyGUI::Edit* _value);
		static MyGUI::Edit* From(EditBox^ _value);
	};
   
	ref class HScrollBar;
	template <> struct Convert<MyGUI::HScroll*>
	{
		typedef HScrollBar^ Type;
		static HScrollBar^ To(MyGUI::HScroll* _value);
		static MyGUI::HScroll* From(HScrollBar^ _value);
	};
   
	ref class ItemBox;
	template <> struct Convert<MyGUI::ItemBox*>
	{
		typedef ItemBox^ Type;
		static ItemBox^ To(MyGUI::ItemBox* _value);
		static MyGUI::ItemBox* From(ItemBox^ _value);
	};
