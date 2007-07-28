#pragma once

#include <vector>
#include <Ogre.h>

namespace widget
{
	//Bit flags done easy
	#define FLAG_NONE  0
	#define FLAG(num)  (1<<(num))

	enum WIDGET_ALIGN {

		ALIGN_NONE						= FLAG_NONE,					// 
		ALIGN_HCENTER					= FLAG_NONE,					// center the window horizontally
		ALIGN_VCENTER					= FLAG_NONE,					// center the window vertically
		ALIGN_CENTER					= ALIGN_HCENTER | ALIGN_VCENTER,		// center the window in the dead center
		ALIGN_CENTER_PARENT		    = FLAG(0),						// это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения
		ALIGN_LEFT						= FLAG(1),						// justify left
		ALIGN_RIGHT					= FLAG(2),						// justify right
		ALIGN_HSTRETCH				    = ALIGN_LEFT | ALIGN_RIGHT,			// stretch to fill the entire parent window horizontally (?)
		ALIGN_TOP						= FLAG(3),						// align from the top
		ALIGN_BOTTOM					= FLAG(4),						// align from the bottom
		ALIGN_VSTRETCH				    = ALIGN_TOP | ALIGN_BOTTOM,			// stretch to fill the entire parent window vertically (?)
		ALIGN_STRETCH					= ALIGN_HSTRETCH | ALIGN_VSTRETCH,	 // stretch to fill the entire parent (?)
	};

	class SubWidget
	{

	public:
		typedef std::vector<SubWidget *> skinChild;
		typedef skinChild::iterator skinIterator;

	public:
		SubWidget(int _x, int _y, int _cx, int _cy, char _align, SubWidget * _parent) :
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
		virtual ~SubWidget() {};

		virtual void move(int _x, int _y) {};
		virtual void move(int _x, int _y, int _cx, int _cy) {};
		virtual void size(int _cx, int _cy) {};

		virtual void update() {};
		virtual void correct() {};
		virtual void show(bool _show) {};
		virtual void align(int _cx, int _cy, bool _update) {};
		virtual void align(int _x, int _y, int _cx, int _cy, bool _update) {};

		virtual void attach(Ogre::OverlayElement * _element) {};

		virtual void setCaption(const Ogre::DisplayString & _caption) {};

		virtual void addUVSet(float _left, float _top, float _right, float _bottom) {};
		virtual void setUVSet(size_t _num) {};

		inline SubWidget * getParent() {return m_parent;}

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

		SubWidget * m_parent;
		bool m_show;
		char m_align;

	}; // class SubWidget

} // namespace widget