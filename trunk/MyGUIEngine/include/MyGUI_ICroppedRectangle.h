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

	class _MyGUIExport ICroppedRectangle
	{

	public:
		ICroppedRectangle(const IntCoord & _coord, Align _align, ICroppedRectangle * _parent) :
			mIsMargin(false),
			mCoord(_coord),
			mParent (_parent),
			mShow(true),
			mAlign (_align)
		{ }
		virtual ~ICroppedRectangle() { }

		virtual void setPosition(const IntPoint& _pos) { }
		virtual void setCoord(const IntCoord& _coord) { }
		virtual void setSize(const IntSize& _size) { }

		virtual void show() { }
		virtual void hide() { }
		virtual bool isShow() { return mShow; }

		virtual void setAlpha(float _alpha) { }
		virtual float getAlpha() { return ALPHA_MAX; }

		ICroppedRectangle * getParent() { return mParent; }

		const IntCoord& getCoord()       { return mCoord; }
		IntPoint getPosition()       { return mCoord.point(); }
		IntSize getSize()       { return mCoord.size(); }

		const IntPoint& getAbsolutePosition() { return mAbsolutePosition; }
		IntRect getAbsoluteRect() { return IntRect(mAbsolutePosition.left, mAbsolutePosition.top, mAbsolutePosition.left+mCoord.width, mAbsolutePosition.top+mCoord.height); }
		IntCoord getAbsoluteCoord() { return IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height); }

		float getAbsoluteLeft() { return mAbsolutePosition.left; }
		float getAbsoluteTop() { return mAbsolutePosition.top; }

		void setAlign(Align _align) { mAlign = _align; }
		Align getAlign() { return mAlign; }

		bool isMargin() { return mIsMargin; }

		int getLeft() { return mCoord.left; }
		int getRight() { return mCoord.right(); }
		int getTop() { return mCoord.top; }
		int getBottom() { return mCoord.bottom(); }
		int getWidth() { return mCoord.width; }
		int getHeight() { return mCoord.height; }

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
			if (getLeft() < mParent->mMargin.left) {
				mMargin.left = mParent->mMargin.left - getLeft();
				margin = true;
			}
			else {
				mMargin.left = 0;
			}

			//вылезли ли направо
			if (getRight() > mParent->getWidth() - mParent->mMargin.right) {
				mMargin.right = getRight() - (mParent->getWidth() - mParent->mMargin.right);
				margin = true;
			}
			else {
				mMargin.right = 0;
			}

			//вылезли ли вверх
			if (getTop() < mParent->mMargin.top) {
				mMargin.top = mParent->mMargin.top - getTop();
				margin = true;
			}
			else {
				mMargin.top = 0;
			}

			//вылезли ли вниз
			if (getBottom() > mParent->getHeight() - mParent->mMargin.bottom) {
				mMargin.bottom = getBottom() - (mParent->getHeight() - mParent->mMargin.bottom);
				margin = true;
			}
			else {
				mMargin.bottom = 0;
			}

			return margin;
		}

		bool _checkOutside() // проверка на полный выход за границу
		{
			return ( (getRight() < mParent->mMargin.left ) || // совсем уехали налево
				(getLeft() > mParent->getWidth() - mParent->mMargin.right ) || // совсем уехали направо
				(getBottom() < mParent->mMargin.top  ) || // совсем уехали вверх
				(getTop() > mParent->getHeight() - mParent->mMargin.bottom ) );  // совсем уехали вниз
		}

	protected:
		bool mIsMargin;
		IntRect mMargin; // перекрытие
		IntCoord mCoord; // координаты
		IntPoint mAbsolutePosition; // обсолютные координаты

		ICroppedRectangle * mParent;
		bool mShow;
		Align mAlign;

	};

} // namespace MyGUI

#endif // __MYGUI_I_CROPPED_RECTANGLE_H__
