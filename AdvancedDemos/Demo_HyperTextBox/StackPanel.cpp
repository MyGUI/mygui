/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "StackPanel.h"

namespace MyGUI
{

	IntSize StackPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		for (Widget* child : getChildWidgets())
		{
			Panel::updateMeasure(child, _sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			result.width = std::max(result.width, size.width);
			result.height += size.height;
		}

		size_t count = getChildCount();
		if (count > 0)
			result.height += (count - 1) * mSpacer.height;

		return result;
	}

	void StackPanel::overrideArrange()
	{
		int offset = 0;

		for (MyGUI::Widget* child : getChildWidgets())
		{
			const IntSize& childSize = Panel::getDesiredSize(child);
			IntCoord coord;

			int height = childSize.height;
			coord.set(0, offset, getWidth(), height);

			Panel::updateArrange(child, coord);

			offset += height + mSpacer.height;
		}
	}

	const IntSize& StackPanel::getSpacer() const
	{
		return mSpacer;
	}

	void StackPanel::setSpacer(const IntSize& _value)
	{
		mSpacer = _value;
	}

} // namespace MyGUI
