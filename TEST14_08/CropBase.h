#pragma once

#include <vector>
#include <Ogre.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgreOverlayElement.h>
#include <OgrePanelOverlayElement.h>

namespace widget
{
	using namespace Ogre;

	class CropBase
	{

	public:
		CropBase(int _x, int _y, int _cx, int _cy, CropBase * _parent);
		virtual ~CropBase();

		CropBase * createChild(int _x, int _y, int _cx, int _cy);

		void size(int _cx, int _cy);
		void move(int _x, int _y);

		void createOverlay(Ogre::Overlay * _overlay, Ogre::OverlayContainer * _overlayContainer, const Ogre::String & material);

	public:
		Ogre::PanelOverlayElement * m_overlayContainer;

	protected:
		void check(); // проверка на выход за границы и обрезка
		void update(); // обновления себя и детей

	protected:
		bool created;

		CropBase * m_parent;
		std::vector<CropBase*> m_widgetChild;

		int m_x, m_y, m_cx, m_cy; // координаты и ширина с высотой
		int m_left_margin, m_right_margin, m_top_margin, m_bottom_margin; // перекрытие

		Real UV_lft_base, UV_rgt_base, UV_top_base, UV_btm_base;

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

		inline void hide(){m_overlayContainer->hide();}
		inline void show(){m_overlayContainer->show();}

	}; // class CropBase

} // namespace widget