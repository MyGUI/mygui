#pragma once

#include <MyGUI.h>

namespace demo
{
	class Widget;

	class Skinable
	{
	public:
		Skinable();
		virtual ~Skinable();

		MyGUI::WidgetStyle getWidgetStyle();
		void setWidgetStyle(MyGUI::WidgetStyle _value);

	protected:
		void onVisualChildAdded(Widget* _child);
		void onVisualChildRemoved(Widget* _child);

		void onDestroySkin();
		void onCreateSkin(const std::string& _skin);

	private:
		MyGUI::WidgetStyle mWidgetStyle;
	};
}