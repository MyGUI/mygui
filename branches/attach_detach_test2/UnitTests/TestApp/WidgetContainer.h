#pragma once

#include <vector>

namespace demo
{
	class Widget;
	typedef std::vector<Widget*> VectorWidgetPtr;

	class WidgetContainer
	{
	public:
		virtual ~WidgetContainer() { }

		virtual void detachWidget(Widget* _widget) = 0;
		virtual void attachWidget(Widget* _widget) = 0;
	};
}