#pragma once

//#include <vector>
#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include "Widget.h"

/*
Основные параметры саб скина
Не имеет детей
Отец Widget
Два метода update:
	1. update если он во весь Widget отца, то не проверять самому выравнивание а брать  готовые результаты у отца
	2. если он сабскин, то тоже проверять свой вьюпорт
*/

namespace widget
{
	using namespace Ogre;

	class Widget;

	class SubSkin
	{

	public:
		SubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, Widget * _parent, PanelOverlayElement * _overlayContainer);
		virtual ~SubSkin();

		void size(int _cx, int _cy);
		void move(int _x, int _y);

		Ogre::PanelOverlayElement * m_overlayContainer;
		String m_material;

		void check(); // проверка на выход за границы и обрезка
		void update(); // обновления себя и детей

		Widget * m_parent;

		int m_x, m_y, m_cx, m_cy; // координаты и ширина с высотой
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие

		float m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV;

		inline int left()       {return m_x;}
		inline int right()      {return m_x + m_cx;}
		inline int top()        {return m_y;}
		inline int bottom()     {return m_y + m_cy;}
		inline int view_left()  {return m_x + m_left_margin;}
		inline int view_right() {return m_x + m_cx - m_right_margin;}
		inline int view_top()   {return m_y + m_top_margin;}
		inline int view_bottom(){return m_y + m_cy - m_bottom_margin;}

		inline int view_width() {return m_cx - m_left_margin - m_right_margin;}
		inline int view_height(){return m_cy - m_top_margin - m_bottom_margin;}

		inline void hide() {m_overlayContainer->hide();}
		inline void show() {m_overlayContainer->show();}

	}; // class SubSkin

} // namespace widget