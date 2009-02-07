
	ref class #{ThisName};
	template <> struct Convert<MyGUI::#{ThisType}*>
	{
		typedef #{ThisName}^ Type;
		static #{ThisName}^ To(MyGUI::#{ThisType}* _value);
		static MyGUI::#{ThisType}* From(#{ThisName}^ _value);
	};
