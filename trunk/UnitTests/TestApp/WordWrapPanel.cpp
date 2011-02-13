/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "WordWrapPanel.h"
#include "MyGUI_TextBox.h"

namespace MyGUI
{

	WordWrapPanel::WordWrapPanel() :
		mOldWidth(0)
	{
	}

	void WordWrapPanel::setPosition(const IntPoint& _value)
	{
		Base::setPosition(_value);
	}

	void WordWrapPanel::setSize(const IntSize& _value)
	{
		Base::setSize(_value);

		onSizeChanged(_value);
	}

	void WordWrapPanel::setCoord(const IntCoord& _value)
	{
		Base::setCoord(_value);

		onSizeChanged(_value.size());
	}

	void WordWrapPanel::onSizeChanged(const IntSize& _size)
	{
		if (mOldWidth != _size.width)
		{
			mOldWidth = _size.width;
			updateContent();
		}
	}

	void WordWrapPanel::updateContent()
	{
		int currentWidth = 0;
		int currentHeight = 0;
		int maxWidth = getWidth();
		int maxLineHeight = 0;
		bool hasWidget = false;
		size_t startLineIndex = 0;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = getWidgetSize(child);

			if (((currentWidth + size.width) > maxWidth) && hasWidget)
			{
				alignChildLine(startLineIndex, index, currentHeight, maxLineHeight);

				currentHeight += maxLineHeight;
				currentWidth = size.width;
				maxLineHeight = size.height;

				startLineIndex = index;
			}
			else
			{
				currentWidth += size.width;
				if (size.height > maxLineHeight)
					maxLineHeight = size.height;
			}

			hasWidget = true;
		}

		if (hasWidget)
			alignChildLine(startLineIndex, count, currentHeight, maxLineHeight);
	}

	void WordWrapPanel::onWidgetCreated(Widget* _widget)
	{
		Base::onWidgetCreated(_widget);

		updateContent();
	}

	void WordWrapPanel::alignChildLine(size_t _startIndex, size_t _stopIndex, int _top, int _height)
	{
		int currentWidth = 0;
		for (size_t index = _startIndex; index < _stopIndex; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = getWidgetSize(child);

			child->setCoord(currentWidth, _top + _height - size.height, size.width, size.height);
			currentWidth += size.width;
		}
	}

	IntSize WordWrapPanel::getWidgetSize(Widget* _widget)
	{
		TextBox* text = _widget->castType<TextBox>(false);
		if (text != nullptr)
			return text->getSize() - text->getTextRegion().size() + text->getTextSize();

		return _widget->getSize();
	}

} // namespace MyGUI
