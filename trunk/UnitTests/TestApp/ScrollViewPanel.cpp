/*!
	@file
	@author		Albert Semenov
	@date		02/2011
*/

#include "ScrollViewPanel.h"
#include <MyGUI_ScrollView.h>

namespace MyGUI
{

	ScrollViewPanel::ScrollViewPanel() :
		mScrollView(nullptr)
	{
	}

	void ScrollViewPanel::initialiseOverride()
	{
		Base::initialiseOverride();

		mScrollView = createWidget<ScrollView>("ScrollView", IntCoord(0, 0, getWidth(), getHeight()), Align::Stretch);
	}

	void ScrollViewPanel::shutdownOverride()
	{
		_destroyChildWidget(mScrollView);
		mScrollView = nullptr;

		Base::shutdownOverride();
	}

	IntSize ScrollViewPanel::overrideMeasure(const IntSize& _sizeAvailable)
	{
		IntSize result;

		size_t count = getChildCount();
		for (size_t index = 0; index < count; ++ index)
		{
			Widget* child = getChildAt(index);
			Panel::updateMeasure(child, _sizeAvailable);
			IntSize size = Panel::getDesiredSize(child);

			result.width = std::max(result.width, size.width);
			result.height += size.height;
		}

		return result;
	}

	void ScrollViewPanel::overrideArrange()
	{
		int offset = 0;

		EnumeratorWidgetPtr child = getEnumerator();
		while (child.next())
		{
			const IntSize& childSize = Panel::getDesiredSize(child.current());
			IntCoord coord;

			int height = childSize.height;
			coord.set(0, offset, getWidth(), height);
			offset += height;

			Panel::updateArrange(child.current(), coord);
		}
	}

} // namespace MyGUI
