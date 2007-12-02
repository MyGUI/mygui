/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__
#define __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include <OgreOverlayElement.h>
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport CroppedRectangleInterface
	{

	public:
		CroppedRectangleInterface(int _left, int _top, int _width, int _height, Align _align, CroppedRectanglePtr _parent) :
			mParent (_parent),
			mAlign (_align),
			mLeft (_left), mTop (_top), mWidth (_width), mHeight (_height),
			mLeftMargin (0), mRightMargin (0), mTopMargin (0), mBottomMargin (0),
			mMargin(false),
			mShow(true)
		{}
		virtual ~CroppedRectangleInterface() {}

		virtual void setPosition(int _left, int _top) {}
		virtual void setPosition(int _left, int _top, int _width, int _height) {}
		virtual void setSize(int _width, int _height) {}

		virtual void show() {}
		virtual void hide() {}
		virtual bool isShow() {return mShow;}

		virtual void setAlpha(float _alpha) {}
		virtual float getAlpha() {return 1.0;}

		inline CroppedRectanglePtr getParent() {return mParent;}

		inline int getLeft()       {return mLeft;}
		inline int getRight()      {return mLeft + mWidth;}
		inline int getTop()        {return mTop;}
		inline int getBottom()     {return mTop + mHeight;}
		inline int getWidth()       {return mWidth;}
		inline int getHeight()       {return mHeight;}

		inline int getViewLeft()  {return mLeft + mLeftMargin;}
		inline int getViewRight() {return mLeft + mWidth - mRightMargin;}
		inline int getViewTop()   {return mTop + mTopMargin;}
		inline int getViewBottom() {return mTop + mHeight - mBottomMargin;}
		inline int getViewWidth() {return mWidth - mLeftMargin - mRightMargin;}
		inline int getViewHeight() {return mHeight - mTopMargin - mBottomMargin;}

		inline int getMarginLeft() {return mLeftMargin;}
		inline int getMarginRight() {return mRightMargin;}
		inline int getMarginTop() {return mTopMargin;}
		inline int getMarginBottom() {return mBottomMargin;}

		virtual void _updateView() {}
		virtual void _correctView() {}
		virtual void _setAlign(int _width, int _height, bool _update) {}
		virtual void _setAlign(int _left, int _top, int _width, int _height, bool _update) {}

		virtual void _attachChild(CroppedRectanglePtr _basis, bool _child) {}

		// вспомогательные методы
		virtual bool _isText() {return false;}
		virtual Ogre::OverlayElement* _getOverlayElement() {return null;}
		virtual size_t _getCountSharedOverlay() {return 0;}
		virtual Ogre::OverlayElement* _getSharedOverlayElement() {return null;}
		virtual void _setUVSet(const FloatRect & _rect) {}

	protected:
		inline bool _checkPoint(int _left, int _top)
		{
			return ! ((getViewLeft() > _left ) || (getViewTop() > _top) || (getViewRight() < _left) || (getViewBottom() < _top) );
		}

		inline bool _checkMargin()
		{
			bool margin = false;
			//вылезли ли налево
			if (getLeft() <= mParent->mLeftMargin) {
				mLeftMargin = mParent->mLeftMargin - getLeft();
				margin = true;
			} else mLeftMargin = 0;

			//вылезли ли направо
			if (getRight() >= mParent->getWidth() - mParent->mRightMargin) {
				mRightMargin = getRight() - (mParent->getWidth() - mParent->mRightMargin);
				margin = true;
			} else mRightMargin = 0;

			//вылезли ли вверх
			if (getTop() <= mParent->mTopMargin) {
				mTopMargin = mParent->mTopMargin - getTop();
				margin = true;
			} else mTopMargin = 0;

			//вылезли ли вниз
			if (getBottom() >= mParent->getHeight() - mParent->mBottomMargin) {
				mBottomMargin = getBottom() - (mParent->getHeight() - mParent->mBottomMargin);
				margin = true;
			} else mBottomMargin = 0;

			return margin;
		}

		inline bool _checkOutside() // проверка на полный выход за границу
		{
			return ( (getRight() <= mParent->mLeftMargin ) || // совсем уехали налево
				(getLeft() >= mParent->getWidth() - mParent->mRightMargin ) || // совсем уехали направо
				(getBottom() <= mParent->mTopMargin  ) || // совсем уехали вверх
				(getTop() >= mParent->getHeight() - mParent->mBottomMargin ) );  // совсем уехали вниз
		}

	protected:

		bool mMargin;
		int mLeft, mTop, mWidth, mHeight; // координаты и ширина с высотой
		int mLeftMargin, mRightMargin, mTopMargin, mBottomMargin; // перекрытие

		CroppedRectanglePtr mParent;
		bool mShow;
		Align mAlign;

	}; // class CroppedRectangleInterface

} // namespace MyGUI

#endif // __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__