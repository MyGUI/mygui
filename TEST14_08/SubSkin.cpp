
#include "SubSkin.h"

namespace widget
{

	SubSkin::SubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, Widget * _parent, PanelOverlayElement * _overlayContainer)
		: m_parent (_parent),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
		m_left_margin (0),
		m_right_margin (0),
		m_top_margin (0),
		m_bottom_margin (0),
		m_baseLeftUV (_leftUV),
		m_baseTopUV (_topUV),
		m_baseRightUV (_rightUV),
		m_baseBottomUV (_bottomUV),
		m_material(_material)
	{

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement(
			"Panel", "Widget_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(m_material);

		if (_overlayContainer) {
			// если у отца есть хоть один сабскин, то оформляем отцовство
			_overlayContainer->addChild(m_overlayContainer);
		} else {
			m_overlayContainer->setPosition(m_parent->m_x + m_x, m_parent->m_y + m_y);
			// а если нет, то создадим отца
			Overlay * overlay;
			overlay = overlayManager.create("WidgetOverlay");
			overlay->setZOrder(200);
			overlay->show();
			overlay->add2D(m_overlayContainer);
		}

		update();
	}

	SubSkin::~SubSkin()
	{
	}

	void SubSkin::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		update();
	}

	void SubSkin::move(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;
		update();
	}

	void SubSkin::check()
	{
		//смотрим, как порубать оверлей
		m_left_margin   = (left()   < m_parent->m_left_margin) ?
									m_parent->m_left_margin - left() : 0; //вылезли ли налево
		m_right_margin  = (right()  > m_parent->m_cx - m_parent->m_right_margin) ?
				    right() - (m_parent->m_cx - m_parent->m_right_margin) : 0; //вылезли ли направо
		m_top_margin    = (top()    < m_parent->m_top_margin) ?
									  m_parent->m_top_margin - top() : 0; //вылезли ли вверх
		m_bottom_margin = (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) ?
				  bottom() - (m_parent->m_cy - m_parent->m_bottom_margin) : 0; //вылезли ли вниз

		// hide не юзать, так как все дочки юзают первый сабскин, используй setTransparent

		if (right()  < m_parent->m_left_margin )                    { m_overlayContainer->setTransparent(true); return;} // совсем уехали налево
		if (left()   > m_parent->m_cx - m_parent->m_right_margin )  { m_overlayContainer->setTransparent(true); return;} // совсем уехали направо
		if (bottom() < m_parent->m_top_margin  )                    { m_overlayContainer->setTransparent(true); return;} // совсем уехали вверх
		if (top()    > m_parent->m_cy - m_parent->m_bottom_margin ) { m_overlayContainer->setTransparent(true); return;} // совсем уехали вниз

		m_overlayContainer->setTransparent(false); // еще что-то видно

		//порубали оверлей
		m_overlayContainer->setPosition(view_left() - m_parent->m_left_margin, view_top() - m_parent->m_top_margin);
		m_overlayContainer->setWidth(view_width());
		m_overlayContainer->setHeight(view_height());

		if(!(m_left_margin || m_right_margin || m_top_margin || m_bottom_margin)){
			//если никуда не вылезли
			m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);
			return;
		}
		// теперь смещаем текстуру
		Real UV_lft = m_left_margin;
		Real UV_top = m_top_margin;
		Real UV_rgt = m_cx - m_right_margin;
		Real UV_btm = m_cy - m_bottom_margin;

		UV_lft = UV_lft / (float)m_cx;
		UV_top = UV_top / (float)m_cy;
		UV_rgt = UV_rgt / (float)m_cx;
		UV_btm = UV_btm / (float)m_cy;

		Real UV_sizeX = m_baseRightUV - m_baseLeftUV;
		Real UV_sizeY = m_baseBottomUV - m_baseTopUV;

		Real UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
		Real UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
		Real UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
		Real UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

		m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
	}

	void SubSkin::update()
	{
		check();
	}


} // namespace SubSkin