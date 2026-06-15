#ifndef _21378433_c369_4d76_a040_70432196a9db_
#define _21378433_c369_4d76_a040_70432196a9db_

#include <MyGUI.h>

namespace tools
{

	struct WidgetStyle;
	using VectorWidgetType = std::vector<WidgetStyle*>;

	struct WidgetStyle
	{
		WidgetStyle(std::string_view _name) :
			name(_name)
		{
		}

		std::string name;
		std::string base;
		using VectorString = std::vector<std::string>;
		VectorString skin;
		std::string default_skin;
		MyGUI::VectorStringPairs parameter;
		MyGUI::VectorStringPairs templateData;
		MyGUI::VectorStringPairs parameterData;
		// can have child widgets
		bool parent{false};
		// can have farent
		bool child{true};
		// can be selected
		bool resizeable{true};
		// have many items (such as ListBox, ComboBox, TabControl, MenuBar, etc...)
		bool many_items{false};
		// hierarchy depths
		size_t depth{0};
		bool internalType{false};
	};

}

#endif
