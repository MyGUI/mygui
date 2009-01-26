/*!
	@file
	@author		Georgiy Evmenov
	@date		11/2007
	@module
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

		virtual void setCoord(const IntCoord& _coord) { mCoord = _coord; }
		const IntCoord& getCoord() { return mCoord; }

		virtual void setPosition(const IntPoint& _pos) { mCoord.left = _pos.left; mCoord.top = _pos.top; }
		IntPoint getPosition() { return mCoord.point(); }

		virtual void setSize(const IntSize& _size) { mCoord.width = _size.width; mCoord.height = _size.height; }
		IntSize getSize() { return mCoord.size(); }

		virtual void setVisible(bool _visible) { mVisible = _visible; }
		bool isVisible() { return mVisible; }

		MYGUI_OBSOLETE("use : void ICroppedRectangle::setVisible(bool _visible)")
		void show() { setVisible(true); }
		MYGUI_OBSOLETE("use : void ICroppedRectangle::setVisible(bool _visible)")
		void hide() { setVisible(false); }
		MYGUI_OBSOLETE("use : bool ICroppedRectangle::isVisible()")
		bool isShow() { return isVisible(); }

		ICroppedRectangle * getCroppedParent() { return mCroppedParent; }

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

		/** True if rectangle is cropped by parent rectangle */
		bool isMargin() { return mIsMargin; }

		// Get coordinates
		int getLeft() { return mCoord.left; }
		int getRight() { return mCoord.right(); }
		int getTop() { return mCoord.top; }
		int getBottom() { return mCoord.bottom(); }
		int getWidth() { return mCoord.width; }
		int getHeight() { return mCoord.height; }

		// Get cropped by parent rectangle coordinates
		int getViewLeft() { return mCoord.left + mMargin.left; }
		int getViewRight() { return mCoord.right() - mMargin.right; }
		int getViewTop() { return mCoord.top + mMargin.top; }
		int getViewBottom() { return mCoord.bottom() - mMargin.bottom; }
		int getViewWidth() { return mCoord.width - mMargin.left - mMargin.right; }
		int getViewHeight() { return mCoord.height - mMargin.top - mMargin.bottom; }

		const IntRect& getMargin() { return mMargin; }
		int getMarginLeft() { return mMargin.left; }
		int getMarginRight() { return mMargin.right; }
		int getMarginTop() { return mMargin.top; }
		int getMarginBottom() { return mMargin.bottom; }

		virtual void _updateView() { }
		virtual void _correctView() { }
		virtual void _setAlign(const IntSize& _size, bool _update)  { }
		virtual void _setAlign(const IntCoord& _coord, bool _update) { }

	protected:
		bool _checkPoint(int _left, int _top)
		{
			return ! ((getViewLeft() > _left ) || (getViewTop() > _top) || (getViewRight() < _left) || (getViewBottom() < _top) );
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
