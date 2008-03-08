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
#include <OgreOverlayElement.h>
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{

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

		inline CroppedRectanglePtr getParent() {return mParent;}

		inline IntPoint getPosition()       {return IntPoint(mCoord.left, mCoord.top);}
		inline IntSize getSize()       {return IntSize(mCoord.width, mCoord.height);}
		inline const IntCoord& getCoord()       {return mCoord;}
		inline Align getAlign() {return mAlign;}
		inline void setAlign(Align _align) {mAlign = _align;}

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

		virtual void _attachChild(CroppedRectanglePtr _basis, bool _child) {}

		// вспомогательные методы
		virtual bool _isText() {return false;}
		virtual Ogre::OverlayElement* _getOverlayElement() {return null;}
		virtual size_t _getCountSharedOverlay() {return 0;}
		virtual Ogre::OverlayElement* _getSharedOverlayElement() {return null;}
		virtual void _setUVSet(const FloatRect& _rect) {}
		virtual void _setMaterialName(const Ogre::String& _material) {}

	protected:
		inline bool _checkPoint(int _left, int _top)
		{
			return ! ((getViewLeft() > _left ) || (getViewTop() > _top) || (getViewRight() < _left) || (getViewBottom() < _top) );
		}

		inline bool _checkMargin()
		{
			bool margin = false;
			//вылезли ли налево
			if (getLeft() <= mParent->mMargin.left) {
				mMargin.left = mParent->mMargin.left - getLeft();
				margin = true;
			} else mMargin.left = 0;

			//вылезли ли направо
			if (getRight() >= mParent->getWidth() - mParent->mMargin.right) {
				mMargin.right = getRight() - (mParent->getWidth() - mParent->mMargin.right);
				margin = true;
			} else mMargin.right = 0;

			//вылезли ли вверх
			if (getTop() <= mParent->mMargin.top) {
				mMargin.top = mParent->mMargin.top - getTop();
				margin = true;
			} else mMargin.top = 0;

			//вылезли ли вниз
			if (getBottom() >= mParent->getHeight() - mParent->mMargin.bottom) {
				mMargin.bottom = getBottom() - (mParent->getHeight() - mParent->mMargin.bottom);
				margin = true;
			} else mMargin.bottom = 0;

			return margin;
		}

		inline bool _checkOutside() // проверка на полный выход за границу
		{
			return ( (getRight() <= mParent->mMargin.left ) || // совсем уехали налево
				(getLeft() >= mParent->getWidth() - mParent->mMargin.right ) || // совсем уехали направо
				(getBottom() <= mParent->mMargin.top  ) || // совсем уехали вверх
				(getTop() >= mParent->getHeight() - mParent->mMargin.bottom ) );  // совсем уехали вниз
		}

	protected:

		bool mIsMargin;
		IntRect mMargin; // перекрытие
		IntCoord mCoord; // координаты

		CroppedRectanglePtr mParent;
		bool mShow;
		Align mAlign;

	}; // class CroppedRectangleInterface

} // namespace MyGUI

#endif // __MYGUI_CROPPED_RECTANGLE_INTERFACE_H__
