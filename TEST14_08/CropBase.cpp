#include "WidgetBasis.h"
#include "MyGUI_Source//MyGUI_AssetManager.h"

namespace widget
{
	CropBase::CropBase(int _x, int _y, int _cx, int _cy, CropBase * _parent)
		: m_parent (_parent),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
		m_left_margin (0),
		m_right_margin (0),
		m_top_margin (0),
		m_bottom_margin (0),
		created(0)
	{
		UV_lft_base = 0.2;
		UV_rgt_base = 0.6;
		UV_top_base = 0.2;
		UV_btm_base = 0.8;
	}

	CropBase::~CropBase()
	{
		while (m_widgetChild.size() > 0)  {
			delete m_widgetChild[0];
			m_widgetChild.pop_back();
		};
	}

	CropBase * CropBase::createChild(int _x, int _y, int _cx, int _cy)
	{
		CropBase * widget = new CropBase(_x, _y, _cx, _cy, this);
		m_widgetChild.push_back(widget);
		update();
		return widget;
	}

	void CropBase::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		update();
	}

	void CropBase::move(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;
//		aligin();
		update();
	}

	void CropBase::check()
	{
		if (!m_parent) return;
		if (!created) return;

		//смотрим, как порубать оверлей
		m_left_margin   = (left()   < m_parent->m_left_margin) ?
									m_parent->m_left_margin - left() : 0; //вылезли ли налево
		m_right_margin  = (right()  > m_parent->m_cx - m_parent->m_right_margin) ?
				    right() - (m_parent->m_cx - m_parent->m_right_margin) : 0; //вылезли ли направо
		m_top_margin    = (top()    < m_parent->m_top_margin) ?
									  m_parent->m_top_margin - top() : 0; //вылезли ли вверх
		m_bottom_margin = (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) ?
				  bottom() - (m_parent->m_cy - m_parent->m_bottom_margin) : 0; //вылезли ли вниз

		if (!m_overlayContainer) return;

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
			m_overlayContainer->setUV(UV_lft_base, UV_top_base, UV_rgt_base, UV_btm_base);
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

		Real UV_sizeX = UV_rgt_base - UV_lft_base;
		Real UV_sizeY = UV_btm_base - UV_top_base;

		Real UV_lft_total = UV_lft_base + UV_lft * UV_sizeX;
		Real UV_top_total = UV_top_base + UV_top * UV_sizeY;
		Real UV_rgt_total = UV_rgt_base - (1-UV_rgt) * UV_sizeX;
		Real UV_btm_total = UV_btm_base - (1-UV_btm) * UV_sizeY;

		m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
	}

	void CropBase::update()
	{
		check();
		std::vector<CropBase*>::iterator iter;
		for(iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++)
			(*iter)->update();
	}

	void CropBase::createOverlay(Ogre::Overlay * _overlay, Ogre::OverlayContainer * _overlayContainer, const Ogre::String & material)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement(
			"Panel", "Widget_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(material);

		if (_overlayContainer) _overlayContainer->addChild(m_overlayContainer);
		else _overlay->add2D(m_overlayContainer);

		created = 1;
		update();
	}


} // namespace CropBase