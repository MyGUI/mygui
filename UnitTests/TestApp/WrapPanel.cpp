/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "WrapPanel.h"
#include "MyGUI_TextBox.h"

namespace MyGUI
{

	WrapPanel::WrapPanel()
	{
	}

	IntSize WrapPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		int maxWidth = _sizeAvailable.width;
		int maxLineHeight = 0;
		bool hasWidget = false;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, _sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			if (((result.width + size.width) > maxWidth) && hasWidget)
			{
				result.height += maxLineHeight;
				result.width = size.width;
				maxLineHeight = size.height;
			}
			else
			{
				result.width += size.width;
				if (size.height > maxLineHeight)
					maxLineHeight = size.height;
			}

			hasWidget = true;
		}

		return result;
	}

	/*void WordWrapPanel::updateContent()
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

		mCalcHeght = currentHeight + maxLineHeight;
	}*/

	void WrapPanel::alignChildLine(size_t _startIndex, size_t _stopIndex, int _top, int _height)
	{
		int currentWidth = 0;
		for (size_t index = _startIndex; index < _stopIndex; ++ index)
		{
			Widget* child = getChildAt(index);
			IntSize size = Panel::getDesiredSize(child);

			Panel::updateArrange(child, IntCoord(currentWidth, _top + _height - size.height, size.width, size.height));
			currentWidth += size.width;
		}
	}

	/*IntSize WordWrapPanel::getWidgetSize(Widget* _widget)
	{
		TextBox* text = _widget->castType<TextBox>(false);
		if (text != nullptr)
			return text->getSize() - text->getTextRegion().size() + text->getTextSize();

		return _widget->getSize();
	}*/

	void WrapPanel::overrideArrange()
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
			IntSize size = Panel::getDesiredSize(child);

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

	/*IntSize WrapPanel::getMaxDistance(EnumeratorWidgetPtr _child, const IntSize& _max)
	{
		IntSize result;
		int current_width = 0;
		int current_height = 0;

		while (_child.next())
		{
			IntSize child_size = Panel::getDesiredSize(_child.current());

			if ((current_width + child_size.width) > _max.width && current_width != 0)
			{
				break;
			}
			result.height = std::max(result.height, child_size.height);
			current_width += child_size.width;
		}

		return result;
	}*/

} // namespace MyGUI
