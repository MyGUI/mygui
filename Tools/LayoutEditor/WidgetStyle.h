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
			name(_name),
			parent(false),
			child(true),
			resizeable(true),
			many_items(false),
			depth(0),
			internalType(false)
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
		bool parent;
		// can have farent
		bool child;
		// can be selected
		bool resizeable;
		// have many items (such as ListBox, ComboBox, TabControl, MenuBar, etc...)
		bool many_items;
		// hierarchy depths
		size_t depth;
		bool internalType;
	};

}

#endif
