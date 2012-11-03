/*!
	@file
	@author		Albert Semenov
	@date		03/2011
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
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

	void ScrollViewPanel::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void ScrollViewPanel::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void ScrollViewPanel::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	void ScrollViewPanel::updateContent()
	{
		// размер клиента с полосами
		IntSize viewSize = mClient->getSize();
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
			viewSize = mClient->getSize();
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
