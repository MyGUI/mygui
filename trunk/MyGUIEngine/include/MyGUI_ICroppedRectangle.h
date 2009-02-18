/*!
	@file
	@author		Georgiy Evmenov
	@date		11/2007
	@module
*//*
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
#include "MyGUI_Common.h"
#include "MyGUI_Align.h"

namespace MyGUI
{

	class MYGUI_EXPORT ICroppedRectangle
	{

	public:
		ICroppedRectangle(const IntCoord & _coord, Align _align, ICroppedRectangle * _croppedParent) :
			mIsMargin(false),
			mCoord(_coord),
			mCroppedParent(_croppedParent),
			mVisible(true),
			mAlign (_align)
		{ }
		virtual ~ICroppedRectangle() { }

		/** Get parent ICroppedRectangle */
		ICroppedRectangle * getCroppedParent() { return mCroppedParent; }

		/** Set coordinates (position and size) */
		virtual void setCoord(const IntCoord& _coord) { mCoord = _coord; }
		/** Get coordinates (position and size) */
		const IntCoord& getCoord() { return mCoord; }

		/** Set position */
		virtual void setPosition(const IntPoint& _pos) { mCoord.left = _pos.left; mCoord.top = _pos.top; }
		/** Get position */
		IntPoint getPosition() { return mCoord.point(); }

		/** Set size */
		virtual void setSize(const IntSize& _size) { mCoord.width = _size.width; mCoord.height = _size.height; }
		/** Get size */
		IntSize getSize() { return mCoord.size(); }

		/** Hide or show */
		virtual void setVisible(bool _visible) { mVisible = _visible; }
		/** Return true if visible */
		bool isVisible() { return mVisible; }

		/** Get position in screen coordinates */
		const IntPoint& getAbsolutePosition() { return mAbsolutePosition; }
		/** Get rectangle in screen coordinates */
		IntRect getAbsoluteRect() { return IntRect(mAbsolutePosition.left, mAbsolutePosition.top, mAbsolutePosition.left+mCoord.width, mAbsolutePosition.top+mCoord.height); }
		/** Get coordinate in screen coordinates */
		IntCoord getAbsoluteCoord() { return IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height); }

		/** Get X in screen coordinates */
		int getAbsoluteLeft() { return mAbsolutePosition.left; }
		/** Get Y in screen coordinates */
		int getAbsoluteTop() { return mAbsolutePosition.top; }

		/** Set align */
		void setAlign(Align _align) { mAlign = _align; }
		/** Get align */
		Align getAlign() { return mAlign; }

		/** Get left x-coordinate */
		int getLeft() { return mCoord.left; }
		/** Get right x-coordinate */
		int getRight() { return mCoord.right(); }
		/** Get top y-coordinate */
		int getTop() { return mCoord.top; }
		/** Get bottom y-coordinate */
		int getBottom() { return mCoord.bottom(); }
		/** Get width */
		int getWidth() { return mCoord.width; }
		/** Get height */
		int getHeight() { return mCoord.height; }


	/*internal:*/
		/** True if rectangle is cropped by parent rectangle */
		bool _isMargin() { return mIsMargin; }

		// Get cropped by parent rectangle coordinates
		int _getViewLeft() { return mCoord.left + mMargin.left; }
		int _getViewRight() { return mCoord.right() - mMargin.right; }
		int _getViewTop() { return mCoord.top + mMargin.top; }
		int _getViewBottom() { return mCoord.bottom() - mMargin.bottom; }
		int _getViewWidth() { return mCoord.width - mMargin.left - mMargin.right; }
		int _getViewHeight() { return mCoord.height - mMargin.top - mMargin.bottom; }

		virtual void _updateView() { }
		virtual void _correctView() { }
		virtual void _setAlign(const IntSize& _size, bool _update)  { }
		virtual void _setAlign(const IntCoord& _coord, bool _update) { }


	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void ICroppedRectangle::setVisible(bool _visible)")
		void show() { setVisible(true); }
		MYGUI_OBSOLETE("use : void ICroppedRectangle::setVisible(bool _visible)")
		void hide() { setVisible(false); }
		MYGUI_OBSOLETE("use : bool ICroppedRectangle::isVisible()")
		bool isShow() { return isVisible(); }

#endif // MYGUI_DONT_USE_OBSOLETE

	protected:
		const IntRect& _getMargin() { return mMargin; }
		int _getMarginLeft() { return mMargin.left; }
		int _getMarginRight() { return mMargin.right; }
		int _getMarginTop() { return mMargin.top; }
		int _getMarginBottom() { return mMargin.bottom; }

		bool _checkPoint(int _left, int _top)
		{
			return ! ((_getViewLeft() > _left ) || (_getViewTop() > _top) || (_getViewRight() < _left) || (_getViewBottom() < _top) );
		}

		bool _checkMargin()
		{
			bool margin = false;
			//вылезли ли налево
			if (getLeft() < mCroppedParent->mMargin.left) {
				mMargin.left = mCroppedParent->mMargin.left - getLeft();
				margin = true;
			}
			else {
				mMargin.left = 0;
			}

			//вылезли ли направо
			if (getRight() > mCroppedParent->getWidth() - mCroppedParent->mMargin.right) {
				mMargin.right = getRight() - (mCroppedParent->getWidth() - mCroppedParent->mMargin.right);
				margin = true;
			}
			else {
				mMargin.right = 0;
			}

			//вылезли ли вверх
			if (getTop() < mCroppedParent->mMargin.top) {
				mMargin.top = mCroppedParent->mMargin.top - getTop();
				margin = true;
			}
			else {
				mMargin.top = 0;
			}

			//вылезли ли вниз
			if (getBottom() > mCroppedParent->getHeight() - mCroppedParent->mMargin.bottom) {
				mMargin.bottom = getBottom() - (mCroppedParent->getHeight() - mCroppedParent->mMargin.bottom);
				margin = true;
			}
			else {
				mMargin.bottom = 0;
			}

			return margin;
		}

		bool _checkOutside() // проверка на полный выход за границу
		{
			return ( (getRight() < mCroppedParent->mMargin.left ) || // совсем уехали налево
				(getLeft() > mCroppedParent->getWidth() - mCroppedParent->mMargin.right ) || // совсем уехали направо
				(getBottom() < mCroppedParent->mMargin.top  ) || // совсем уехали вверх
				(getTop() > mCroppedParent->getHeight() - mCroppedParent->mMargin.bottom ) );  // совсем уехали вниз
		}

	protected:
		bool mIsMargin;
		IntRect mMargin; // перекрытие
		IntCoord mCoord; // координаты
		IntPoint mAbsolutePosition; // обсолютные координаты

		ICroppedRectangle * mCroppedParent;
		bool mVisible;
		Align mAlign;

	};

} // namespace MyGUI

#endif // __MYGUI_I_CROPPED_RECTANGLE_H__
