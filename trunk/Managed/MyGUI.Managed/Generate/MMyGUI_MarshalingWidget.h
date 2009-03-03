   
	ref class Button;
	template <> struct Convert<MyGUI::Button*>
	{
		typedef Button^ Type;
		static Button^ To(MyGUI::Button* _value);
		static MyGUI::Button* From(Button^ _value);
	};
