/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "StackPanel.h"
#include "WordWrapPanel.h"

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
		int currentHeight = 0;
		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			WordWrapPanel* panel = child->castType<WordWrapPanel>(false);
			if (panel != nullptr)
			{
				int height = panel->getHeightByWidth(getWidth());
				currentHeight += height;
			}
		}
	}

	void StackPanel::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		updateContent();
	}

} // namespace MyGUI
