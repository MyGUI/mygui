#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetDefines.h"

namespace widget
{

	class BasisWidget
	{

	public:
		BasisWidget(int _x, int _y, int _cx, int _cy, char _align, BasisWidgetPtr _parent) :
			m_parent (_parent),
			m_align (_align),
			m_x (_x),
			m_y (_y),
			m_cx (_cx),
			m_cy (_cy),
			m_left_margin (0),
			m_right_margin (0),
			m_top_margin (0),
			m_bottom_margin (0),
			m_margin(false),
			m_show(true)
		{};
		virtual ~BasisWidget() {};

		virtual void move(int _x, int _y) {};
		virtual void move(int _x, int _y, int _cx, int _cy) {};
		virtual void size(int _cx, int _cy) {};

		virtual void show(bool _show) {};
		virtual bool isShow() {return m_show;};

		virtual void setCaption(const Ogre::DisplayString & _caption) {};
		virtual const Ogre::DisplayString & getCaption() {static Ogre::DisplayString caption; return caption;};

		virtual void setColour(const Ogre::ColourValue & _color) {};
		virtual void setAlpha(float _alpha) {};

		virtual void setFontName(const Ogre::String & _font) {};
		virtual void setFontName(const Ogre::String & _font, Ogre::ushort _height) {};
		virtual const Ogre::String & getFontName() {static Ogre::String name; return name;};

		virtual void setCharHeight(Ogre::ushort _height) {};
		virtual Ogre::ushort getCharHeight() {return 0;};

		virtual void update() {};
		virtual void correct() {};
		virtual void align(int _cx, int _cy, bool _update) {};
		virtual void align(int _x, int _y, int _cx, int _cy, bool _update) {};

		virtual void attach(OverlayElementPtr _element, bool _child) {};

		virtual void setUVSet(const Ogre::FloatRect & _rect) {};

		virtual bool isText() {return false;};

		inline BasisWidgetPtr getParent() {return m_parent;}

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

		inline bool check_margin ()
		{
			bool margin = false;
			//вылезли ли налево
			if (left()   < m_parent->m_left_margin) {
				m_left_margin = m_parent->m_left_margin - left();
				margin = true;
			} else m_left_margin = 0;

			//вылезли ли направо
			if (right()  > m_parent->width() - m_parent->m_right_margin) {
				m_right_margin = right() - (m_parent->width() - m_parent->m_right_margin);
				margin = true;
			} else m_right_margin = 0;

			//вылезли ли вверх
			if (top()    < m_parent->m_top_margin) {
				m_top_margin = m_parent->m_top_margin - top();
				margin = true;
			} else m_top_margin = 0;

			//вылезли ли вниз
			if (bottom() > m_parent->height() - m_parent->m_bottom_margin) {
				m_bottom_margin = bottom() - (m_parent->height() - m_parent->m_bottom_margin);
				margin = true;
			} else m_bottom_margin = 0;

			return margin;
		}

		inline bool check_outside() // проверка на полный выход за границу
		{
			return ( (right() < m_parent->m_left_margin ) || // совсем уехали налево
				(left() > m_parent->width() - m_parent->m_right_margin ) || // совсем уехали направо
				(bottom() < m_parent->m_top_margin  ) || // совсем уехали вверх
				(top() > m_parent->height() - m_parent->m_bottom_margin ) );  // совсем уехали вниз
		}

	protected:

		bool m_margin;
		int m_x, m_y, m_cx, m_cy; // координаты и ширина с высотой
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие

		BasisWidgetPtr m_parent;
		bool m_show;
		char m_align;

	}; // class BasisWidget

} // namespace widget