/*!
	@file
	@author		Georgiy Evmenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__
#define __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

	class LayerItemKeeper;
	class RenderItem;

	class _MyGUIExport CroppedRectangleInterface
	{

	public:
		CroppedRectangleInterface(const IntCoord& _coord, Align _align, CroppedRectanglePtr _parent) :
			mIsMargin(false),
			mCoord(_coord),
			mParent (_parent),
			mShow(true),
			mAlign (_align)
		{}
		virtual ~CroppedRectangleInterface() {}

		virtual void setPosition(const IntPoint& _pos) {}
		virtual void setPosition(const IntCoord& _coord) {}
		virtual void setSize(const IntSize& _size) {}

		virtual void show() {}
		virtual void hide() {}
		virtual bool isShow() {return mShow;}

		virtual void setAlpha(float _alpha) {}
		virtual float getAlpha() {return ALPHA_MAX;}

		virtual void setColourAlign(const Ogre::ColourValue & _colour, Align _align) {}
		virtual const Ogre::ColourValue & getColourAlign(Align _align) {return Ogre::ColourValue::Black;}

		inline CroppedRectanglePtr getParent() {return mParent;}

		inline const IntCoord& getCoord()       {return mCoord;}
		inline IntPoint getPosition()       {return mCoord.point();}
		inline IntSize getSize()       {return mCoord.size();}

		inline const IntPoint& getAbsolutePosition() {return mAbsolutePosition;}
		inline IntRect getAbsoluteRect() {return IntRect(mAbsolutePosition.left, mAbsolutePosition.top, mAbsolutePosition.left+mCoord.width, mAbsolutePosition.top+mCoord.height);}
		inline IntCoord getAbsoluteCoord() {return IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height);}

		inline float getAbsoluteLeft() {return mAbsolutePosition.left;}
		inline float getAbsoluteTop() {return mAbsolutePosition.top;}

		inline void setAlign(Align _align) {mAlign = _align;}
		inline Align getAlign() {return mAlign;}

		inline bool isMargin() {return mIsMargin;}

		inline int getLeft()       {return mCoord.left;}
		inline int getRight()      {return mCoord.right();}
		inline int getTop()        {return mCoord.top;}
		inline int getBottom()     {return mCoord.bottom();}
		inline int getWidth()       {return mCoord.width;}
		inline int getHeight()       {return mCoord.height;}

		inline int getViewLeft()  {return mCoord.left + mMargin.left;}
		inline int getViewRight() {return mCoord.right() - mMargin.right;}
		inline int getViewTop()   {return mCoord.top + mMargin.top;}
		inline int getViewBottom() {return mCoord.bottom() - mMargin.bottom;}
		inline int getViewWidth() {return mCoord.width - mMargin.left - mMargin.right;}
		inline int getViewHeight() {return mCoord.height - mMargin.top - mMargin.bottom;}

		inline const IntRect& getMargin() {return mMargin;}
		inline int getMarginLeft() {return mMargin.left;}
		inline int getMarginRight() {return mMargin.right;}
		inline int getMarginTop() {return mMargin.top;}
		inline int getMarginBottom() {return mMargin.bottom;}

		virtual void _updateView() {}
		virtual void _correctView() {}
		virtual void _setAlign(const IntSize& _size, bool _update) {}
		virtual void _setAlign(const IntCoord& _coord, bool _update) {}

		// для каста сабскина
		virtual bool _isText() {return false;}

		virtual void _setUVSet(const FloatRect& _rect) {}

		virtual void _createDrawItem(LayerItemKeeper * _keeper, RenderItem * _item) {}
		virtual void _destroyDrawItem() {}
		

	protected:
		inline bool _checkPoint(int _left, int _top)
		{
			return ! ((getViewLeft() > _left ) || (getViewTop() > _top) || (getViewRight() < _left) || (getViewBottom() < _top) );
		}

		inline bool _checkMargin()
		{
			bool margin = false;
			//вылезли ли налево
			if (getLeft() < mParent->mMargin.left) {
				mMargin.left = mParent->mMargin.left - getLeft();
				margin = true;
			} else mMargin.left = 0;

			//вылезли ли направо
			if (getRight() > mParent->getWidth() - mParent->mMargin.right) {
				mMargin.right = getRight() - (mParent->getWidth() - mParent->mMargin.right);
				margin = true;
			} else mMargin.right = 0;

			//вылезли ли вверх
			if (getTop() < mParent->mMargin.top) {
				mMargin.top = mParent->mMargin.top - getTop();
				margin = true;
			} else mMargin.top = 0;

			//вылезли ли вниз
			if (getBottom() > mParent->getHeight() - mParent->mMargin.bottom) {
				mMargin.bottom = getBottom() - (mParent->getHeight() - mParent->mMargin.bottom);
				margin = true;
			} else mMargin.bottom = 0;

			return margin;
		}

		inline bool _checkOutside() // проверка на полный выход за границу
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

		CroppedRectanglePtr mParent;
		bool mShow;
		Align mAlign;

	};

} // namespace MyGUI

#endif // __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__
