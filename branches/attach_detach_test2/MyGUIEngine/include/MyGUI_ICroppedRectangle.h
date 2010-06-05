/*!
	@file
	@author		Georgiy Evmenov
	@date		11/2007
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
#ifndef __MYGUI_I_CROPPED_RECTANGLE_H__
#define __MYGUI_I_CROPPED_RECTANGLE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"

namespace MyGUI
{

	class MYGUI_EXPORT ICroppedRectangle
	{
	public:
		ICroppedRectangle() { }
		virtual ~ICroppedRectangle() { }

		/** Get position */
		IntPoint getPosition() const { return mCoord.point(); }
		/** Get size */
		IntSize getSize() const { return mCoord.size(); }
		/** Get coordinates (position and size) */
		const IntCoord& getCoord() const { return mCoord; }

		/** Get left x-coordinate */
		int getLeft() const { return mCoord.left; }
		/** Get right x-coordinate */
		int getRight() const { return mCoord.right(); }
		/** Get top y-coordinate */
		int getTop() const { return mCoord.top; }
		/** Get bottom y-coordinate */
		int getBottom() const { return mCoord.bottom(); }
		/** Get width */
		int getWidth() const { return mCoord.width; }
		/** Get height */
		int getHeight() const { return mCoord.height; }


	/*internal:*/
		// Get cropped by parent rectangle coordinates
		int _getViewLeft() const { return mCoord.left + mMargin.left; }
		int _getViewRight() const { return mCoord.right() - mMargin.right; }
		int _getViewTop() const { return mCoord.top + mMargin.top; }
		int _getViewBottom() const { return mCoord.bottom() - mMargin.bottom; }
		int _getViewWidth() const { return mCoord.width - mMargin.left - mMargin.right; }
		int _getViewHeight() const { return mCoord.height - mMargin.top - mMargin.bottom; }

		const IntRect& _getMarginRect() const { return mMargin; }
		int _getMarginLeft() const { return mMargin.left; }
		int _getMarginRight() const { return mMargin.right; }
		int _getMarginTop() const { return mMargin.top; }
		int _getMarginBottom() const { return mMargin.bottom; }

	protected:
		bool _checkMargin(ICroppedRectangle* _parent)
		{
			bool margin = false;
			//вылезли ли налево
			if (getLeft() < _parent->mMargin.left)
			{
				mMargin.left = _parent->mMargin.left - getLeft();
				margin = true;
			}
			else
			{
				mMargin.left = 0;
			}

			//вылезли ли направо
			if (getRight() > _parent->getWidth() - _parent->mMargin.right)
			{
				mMargin.right = getRight() - (_parent->getWidth() - _parent->mMargin.right);
				margin = true;
			}
			else
			{
				mMargin.right = 0;
			}

			//вылезли ли вверх
			if (getTop() < _parent->mMargin.top)
			{
				mMargin.top = _parent->mMargin.top - getTop();
				margin = true;
			}
			else
			{
				mMargin.top = 0;
			}

			//вылезли ли вниз
			if (getBottom() > _parent->getHeight() - _parent->mMargin.bottom)
			{
				mMargin.bottom = getBottom() - (_parent->getHeight() - _parent->mMargin.bottom);
				margin = true;
			}
			else
			{
				mMargin.bottom = 0;
			}

			return margin;
		}

		bool _checkOutside(ICroppedRectangle* _parent) // проверка на полный выход за границу
		{
			return ( (getRight() < _parent->mMargin.left ) || // совсем уехали налево
				(getLeft() > _parent->getWidth() - _parent->mMargin.right ) || // совсем уехали направо
				(getBottom() < _parent->mMargin.top  ) || // совсем уехали вверх
				(getTop() > _parent->getHeight() - _parent->mMargin.bottom ) );  // совсем уехали вниз
		}

	private:
		IntRect mMargin; // перекрытие
	protected:
		IntCoord mCoord; // координаты
	};

} // namespace MyGUI

#endif // __MYGUI_I_CROPPED_RECTANGLE_H__
