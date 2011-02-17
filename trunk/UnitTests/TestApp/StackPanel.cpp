/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "StackPanel.h"

namespace MyGUI
{

	StackPanel::StackPanel()
	{
	}

	void StackPanel::setPosition(const IntPoint& _value)
	{
		Base::setPosition(_value);
	}

	void StackPanel::setSize(const IntSize& _value)
	{
		Base::setSize(_value);

		onSizeChanged(_value);
	}

	void StackPanel::setCoord(const IntCoord& _value)
	{
		Base::setCoord(_value);

		onSizeChanged(_value.size());
	}

	void StackPanel::onSizeChanged(const IntSize& _size)
	{
		updateContent();
	}

	void StackPanel::updateContent()
	{
		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			//IntSize size = getWidgetSize(child);
		}
	}

	void StackPanel::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		updateContent();
	}

} // namespace MyGUI
