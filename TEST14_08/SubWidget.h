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
		SubWidget(int _x, int _y, int _cx, int _cy, char _align, SubWidget * _parent);
		virtual ~SubWidget();

		virtual void move(int _x, int _y);
		virtual void move(int _x, int _y, int _cx, int _cy);
		virtual void size(int _cx, int _cy);

		virtual void update();
		virtual void restore();
		virtual void correct();
		virtual void show(bool _show);
		virtual void align(int _cx, int _cy);

		virtual void attach(Ogre::OverlayElement * _element);

		inline int left()       {return m_x;}
		inline int right()      {return m_x + m_cx;}
		inline int top()        {return m_y;}
		inline int bottom()     {return m_y + m_cy;}
		inline int view_left()  {return m_x + m_left_margin;}
		inline int view_right() {return m_x + m_cx - m_right_margin;}
		inline int view_top()   {return m_y + m_top_margin;}
		inline int view_bottom() {return m_y + m_cy - m_bottom_margin;}

		inline int view_width() {return m_cx - m_left_margin - m_right_margin;}
		inline int view_height() {return m_cy - m_top_margin - m_bottom_margin;}

		bool m_margin;
		bool m_correct;
		int m_x, m_y, m_cx, m_cy; // координаты и ширина с высотой
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие

		SubWidget * m_parent;
		bool m_show;
		char m_align;


	}; // class SubWidget

} // namespace widget