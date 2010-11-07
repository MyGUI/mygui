#ifndef __WIDGET_STYLE_H__
#define __WIDGET_STYLE_H__

#include <MyGUI.h>

namespace tools
{
	struct WidgetStyle;
	typedef std::vector<WidgetStyle*> VectorWidgetType;

	struct WidgetStyle
	{
		WidgetStyle(const std::string& _name) :
			name(_name),
			parent(false),
			child(true),
			resizeable(true),
			many_items(false)
		{
		}

		std::string name;
		std::string base;
		typedef std::vector<std::string> VectorString;
		VectorString skin;
		std::string default_skin;
		MyGUI::VectorStringPairs parameter;
		// can have child widgets
		bool parent;
		// can have farent
		bool child;
		// can be selected
		bool resizeable;
		// have many items (such as List, ComboBox, Tab, MenuBar, etc...)
		bool many_items;
	};

} // namespace tools

#endif // __WIDGET_STYLE_H__
