#pragma once

#include <vector>
#include <Ogre.h>
#include "SubSkin.h"

namespace widget
{
	using namespace Ogre;

	class SubSkin;

	class Widget
	{

	public:
		Widget(int _x, int _y, int _cx, int _cy, Widget * _parent);
		virtual ~Widget();

		// создаем дочку
		Widget * createChild(int _x, int _y, int _cx, int _cy);
		// создаем и добавляем саб скин виджету
		void addSubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material);

//		void size(int _cx, int _cy);
		void move(int _x, int _y);

		// показывает скрывает все сабскины
		void showSkins(bool _show);


		void check(); // проверка на выход за границы и обрезка
//		void update(); // обновления себя и детей

		Widget * m_parent;

		typedef std::vector<Widget *> widgetChild;
		typedef widgetChild::iterator widgetIterator;
		typedef std::vector<SubSkin *> skinChild;
		typedef skinChild::iterator skinIterator;

		// вектор всех детей виджетов
		widgetChild m_widgetChild;
		// вектор всех детей сабскинов
		skinChild m_subSkinChild;

		int m_x, m_y, m_cx, m_cy; // координаты и ширина с высотой
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие
		// вылез ли виджет в данный момент
		bool m_margin;
		// скрыты ли все сабскины
		bool m_showSkins;

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

		//		inline void hide() {/*m_overlayContainer->hide();*/}
//		inline void show() {/*m_overlayContainer->show();*/}

	}; // class Widget

} // namespace widget