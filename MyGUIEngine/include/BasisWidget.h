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
		BasisWidget(int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent) :
			mParent (_parent),
			mAlign (_align),
			m_x (_x), m_y (_y), m_cx (_cx), m_cy (_cy),
			m_left_margin (0), m_right_margin (0), m_top_margin (0), m_bottom_margin (0),
			mMargin(false),
			mShow(true)
		{}
		virtual ~BasisWidget() {}

		virtual void move(int _x, int _y) {}
		virtual void move(int _x, int _y, int _cx, int _cy) {}
		virtual void size(int _cx, int _cy) {}

		virtual void show(bool _show) {}
		virtual bool isShow() {return mShow;}

		virtual void setCaption(const Ogre::DisplayString & _caption) {}
		virtual const Ogre::DisplayString & getCaption() {static Ogre::DisplayString caption; return caption;}

		virtual void setColour(const Ogre::ColourValue & _color) {}
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
		virtual void align(int _cx, int _cy, bool _update) {}
		virtual void align(int _x, int _y, int _cx, int _cy, bool _update) {}

		virtual void attach(BasisWidgetPtr _basis, bool _child) {}//???
//		virtual void detach(BasisWidgetPtr _basis/*, bool _child*/) {}

		virtual Ogre::OverlayElement* getOverlayElement() {return 0;}//???

		virtual void setUVSet(const FloatRect & _rect) {}

		virtual bool isText() {return false;}

		inline BasisWidgetPtr getParent() {return mParent;}

		inline int left()       {return m_x;}
		inline int right()      {return m_x + m_cx;}
		inline int top()        {return m_y;}
		inline int bottom()     {return m_y + m_cy;}
		inline int width()       {return m_cx;}
		inline int height()       {return m_cy;}

		inline int view_left()  {return m_x + m_left_margin;}
		inline int view_right() {return m_x + m_cx - m_right_margin;}
		inline int view_top()   {return m_y + m_top_margin;}
		inline int view_bottom() {return m_y + m_cy - m_bottom_margin;}
		inline int view_width() {return m_cx - m_left_margin - m_right_margin;}
		inline int view_height() {return m_cy - m_top_margin - m_bottom_margin;}

		inline int margin_left() {return m_left_margin;}
		inline int margin_right() {return m_right_margin;}
		inline int margin_top() {return m_top_margin;}
		inline int margin_bottom() {return m_bottom_margin;}

		inline bool check_point (int _x, int _y)
		{
//			return ((view_left() <= _x ) && (view_top() <= _y) && (view_right() >= _x) && (view_bottom() >= _y) );
			return ! ((view_left() > _x ) || (view_top() > _y) || (view_right() < _x) || (view_bottom() < _y) );
		}

		inline bool check_margin ()
		{
			bool margin = false;
			//вылезли ли налево
			if (left() <= mParent->m_left_margin) {
				m_left_margin = mParent->m_left_margin - left();
				margin = true;
			} else m_left_margin = 0;

			//вылезли ли направо
			if (right() >= mParent->width() - mParent->m_right_margin) {
				m_right_margin = right() - (mParent->width() - mParent->m_right_margin);
				margin = true;
			} else m_right_margin = 0;

			//вылезли ли вверх
			if (top() <= mParent->m_top_margin) {
				m_top_margin = mParent->m_top_margin - top();
				margin = true;
			} else m_top_margin = 0;

			//вылезли ли вниз
			if (bottom() >= mParent->height() - mParent->m_bottom_margin) {
				m_bottom_margin = bottom() - (mParent->height() - mParent->m_bottom_margin);
				margin = true;
			} else m_bottom_margin = 0;

			return margin;
		}

		inline bool check_outside() // проверка на полный выход за границу
		{
			return ( (right() <= mParent->m_left_margin ) || // совсем уехали налево
				(left() >= mParent->width() - mParent->m_right_margin ) || // совсем уехали направо
				(bottom() <= mParent->m_top_margin  ) || // совсем уехали вверх
				(top() >= mParent->height() - mParent->m_bottom_margin ) );  // совсем уехали вниз
		}

	protected:

		bool mMargin;
		int m_x, m_y, m_cx, m_cy; // координаты и ширина с высотой
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие

		BasisWidgetPtr mParent;
		bool mShow;
		Align mAlign;

	}; // class BasisWidget

} // namespace MyGUI

#endif // __BASIC_WIDGET_H__