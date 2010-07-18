
#include "Skinable.h"
#include "Widget.h"

namespace demo
{
	Skinable::Skinable() :
		mWidgetStyle(MyGUI::WidgetStyle::Child)
	{
	}

	Skinable::~Skinable()
	{
	}

	void Skinable::onVisualChildAdded(Widget* _child)
	{
	}

	void Skinable::onVisualChildRemoved(Widget* _child)
	{
	}

	void Skinable::onDestroySkin()
	{
	}

	void Skinable::onCreateSkin(const std::string& _skin)
	{
	}

	MyGUI::WidgetStyle Skinable::getWidgetStyle()
	{
		return mWidgetStyle;
	}

	void Skinable::setWidgetStyle(MyGUI::WidgetStyle _value)
	{
		mWidgetStyle = _value;
	}

}