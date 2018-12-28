/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/

#include "MyGUI_Precompiled.h"
#include "ScrollViewPanel.h"
#include "Panel.h"
#include "MyGUI_ScrollBar.h"

namespace MyGUI
{

	ScrollViewPanel::ScrollViewPanel()
	{
	}

	void ScrollViewPanel::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void ScrollViewPanel::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		updateContent();
	}

	void ScrollViewPanel::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		updateContent();
	}

	void ScrollViewPanel::updateContent()
	{
		// размер клиента с полосами
		IntSize viewSize = mScrollViewClient->getSize();
		if (!getVScroll()->getVisible())
			viewSize.width -= getVScroll()->getWidth();

		size_t childCount = getChildCount();
		if (childCount == 0)
			return;

		Panel::updateMeasure(getChildAt(0), IntSize(viewSize.width, (std::numeric_limits<int>::max)()));
		IntSize resultSize = Panel::getDesiredSize(getChildAt(0));

		// содержимое влазиет по высоте, вертикального скрола не будет
		if (viewSize.height >= resultSize.height)
		{
			viewSize = mScrollViewClient->getSize();
			if (getVScroll()->getVisible())
				viewSize.width += getVScroll()->getWidth();

			Panel::updateMeasure(getChildAt(0), IntSize(viewSize.width, (std::numeric_limits<int>::max)()));
			IntSize resultSize = Panel::getDesiredSize(getChildAt(0));

			Panel::updateArrange(getChildAt(0), IntCoord(0, 0, viewSize.width, resultSize.height));
			setCanvasSize(IntSize(viewSize.width, resultSize.height));
		}
		// содержимое больше, будет виден вертикальный скрол
		else
		{
			Panel::updateArrange(getChildAt(0), IntCoord(0, 0, viewSize.width, resultSize.height));
			setCanvasSize(IntSize(viewSize.width, resultSize.height));
		}
	}

} // namespace MyGUI
