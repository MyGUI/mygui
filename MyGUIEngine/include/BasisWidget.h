#ifndef __BASIC_WIDGET_H__
#define __BASIC_WIDGET_H__

#include "Prerequest.h"
#include <OgreOverlayElement.h>
#include "WidgetDefines.h"

namespace MyGUI
{

	class _MyGUIExport BasisWidget
	{

	public:
		BasisWidget(int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent) :
			mParent (_parent),
			mAlign (_align),
			mLeft (_left), mTop (_top), mWidth (_width), mHeight (_height),
			mLeftMargin (0), mRightMargin (0), mTopMargin (0), mBottomMargin (0),
			mMargin(false),
			mShow(true)
		{}
		virtual ~BasisWidget() {}

		virtual void move(int _left, int _top) {}
		virtual void move(int _left, int _top, int _width, int _height) {}
		virtual void size(int _width, int _height) {}

		virtual void show(bool _show) {}
		virtual bool isShow() {return mShow;}

		virtual void setCaption(const Ogre::DisplayString & _caption) {}
		virtual const Ogre::DisplayString & getCaption() {static Ogre::DisplayString caption; return caption;}

		virtual void setColour(const Ogre::ColourValue & _colour) {}
		virtual const Ogre::ColourValue & getColour() {return Ogre::ColourValue::Black;}

		virtual void setAlpha(float _alpha) {}
		virtual float getAlpha() {return 1.0;}

		virtual void setFontName(const Ogre::String & _font) {}
		virtual void setFontName(const Ogre::String & _font, Ogre::ushort _height) {}
		virtual const Ogre::String & getFontName() {static Ogre::String name; return name;}

		virtual void setFontHeight(Ogre::ushort _height) {}
		virtual Ogre::ushort getFontHeight() {return 0;}

		virtual void setTextAlign(Align _align) {}
		inline Align getAlign() {return mAlign;}

		virtual void setTextSelect(size_t _start, size_t _end) {}
		// возвращает положение курсора по произвольному положению
		virtual size_t getTextCursorFromPoint(IntPoint & _point) {return 0;}
		// возвращает положение курсора по позиции
		virtual IntPoint getTextCursorFromPosition(size_t _position) {return IntPoint();}
		// возвращает размер текста в пикселях
		virtual IntSize getTextSize() {return IntSize();}
		// устанавливает смещение текста в пикселях
		virtual void setTextShift(IntPoint _point) {}
		virtual IntPoint getTextShift() {return IntPoint();}

		virtual void update() {}
		virtual void correct() {}
		virtual void align(int _width, int _height, bool _update) {}
		virtual void align(int _left, int _top, int _width, int _height, bool _update) {}

		virtual void attach(BasisWidgetPtr _basis, bool _child) {}//???
//		virtual void detach(BasisWidgetPtr _basis/*, bool _child*/) {}

		virtual Ogre::OverlayElement* getOverlayElement() {return 0;}//???

		virtual void setUVSet(const FloatRect & _rect) {}

		virtual bool isText() {return false;}

		inline BasisWidgetPtr getParent() {return mParent;}

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
		inline int view_height() {return mHeight - mTopMargin - mBottomMargin;}

		inline int getMarginLeft() {return mLeftMargin;}
		inline int getMarginRight() {return mRightMargin;}
		inline int getMarginTop() {return mTopMargin;}
		inline int getMarginBottom() {return mBottomMargin;}

		inline bool checkPoint (int _left, int _top)
		{
			return ! ((getViewLeft() > _left ) || (getViewTop() > _top) || (getViewRight() < _left) || (getViewBottom() < _top) );
		}

		inline bool checkMargin ()
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

		inline bool checkOutside() // проверка на полный выход за границу
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

		BasisWidgetPtr mParent;
		bool mShow;
		Align mAlign;

	}; // class BasisWidget

} // namespace MyGUI

#endif // __BASIC_WIDGET_H__