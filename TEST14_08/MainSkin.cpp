
//#include "SubSkin.h"
#include "MainSkin.h"
#include "debugOut.h"

namespace widget
{

	MainSkin::MainSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, SubWidget * _parent) : 
		SubWidget(_x, _y, _cx, _cy, _align, _parent),
		m_baseLeftUV (_leftUV),
		m_baseTopUV (_topUV),
		m_baseRightUV (_rightUV),
		m_baseBottomUV (_bottomUV)
	{

		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<Ogre::PanelOverlayElement*>(overlayManager.createOverlayElement(
			"Panel", "Widget_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_parent->left() + m_x, m_parent->top() + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(_material);

		m_parent->attach(m_overlayContainer);

	}

	MainSkin::~MainSkin()
	{
	}

	void MainSkin::align(int _cx, int _cy, bool _update)
	{

		if (_update) {
			update();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(m_parent->width() < 0) ? m_cx = 0 : m_cx = m_parent->width();
		(m_parent->height() < 0) ? m_cy = 0 : m_cy = m_parent->height();
		m_overlayContainer->setDimensions(m_cx, m_cy);

	}

	void MainSkin::align(int _x, int _y, int _cx, int _cy, bool _update)
	{

		m_overlayContainer->setPosition(m_x + m_parent->left() - m_parent->getParent()->margin_left(), m_y + m_parent->top() - m_parent->getParent()->margin_top());

		if (_update) {
			update();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(m_parent->width() < 0) ? m_cx = 0 : m_cx = m_parent->width();
		(m_parent->height() < 0) ? m_cy = 0 : m_cy = m_parent->height();
		m_overlayContainer->setDimensions(m_cx, m_cy);

	}

	void MainSkin::correct()
	{
		m_overlayContainer->setPosition(m_x + m_parent->left() - m_parent->getParent()->margin_left(), m_y + m_parent->top() - m_parent->getParent()->margin_top());
	}

	void MainSkin::update()
	{
		int cx = m_parent->view_width();
		if (cx < 0) cx = 0;
		int cy = m_parent->view_height();
		if (cy < 0) cy = 0;

		//порубали оверлей
		m_overlayContainer->setPosition(m_parent->view_left() - m_parent->getParent()->margin_left(), m_parent->view_top() - m_parent->getParent()->margin_top());
		m_overlayContainer->setDimensions(cx, cy);

		// теперь смещаем текстуру
		float UV_lft = m_parent->margin_left();
		float UV_top = m_parent->margin_top();
		float UV_rgt = m_parent->width() - m_parent->margin_right();
		float UV_btm = m_parent->height() - m_parent->margin_bottom();

		UV_lft = UV_lft / (float)m_parent->width();
		UV_top = UV_top / (float)m_parent->height();
		UV_rgt = UV_rgt / (float)m_parent->width();
		UV_btm = UV_btm / (float)m_parent->height();

		float UV_sizeX = m_baseRightUV - m_baseLeftUV;
		float UV_sizeY = m_baseBottomUV - m_baseTopUV;

		float UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
		float UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
		float UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
		float UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

		m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

	}

	void MainSkin::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		_show ? m_overlayContainer->show() : m_overlayContainer->hide();
	};

	void MainSkin::attach(Ogre::OverlayElement * _element)
	{
		m_overlayContainer->addChild(_element);
	}

} // namespace MainSkin