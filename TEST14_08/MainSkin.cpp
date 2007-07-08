
#include "SubSkin.h"
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
		m_overlayContainer->setPosition(m_parent->m_x + m_x, m_parent->m_y + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(_material);

		m_parent->attach(m_overlayContainer);

	}

	MainSkin::~MainSkin()
	{
	}

	void MainSkin::move(int _x, int _y)
	{
		// присваеваем оверлею позицию отца плюс смещение внутри отца
		m_overlayContainer->setPosition(m_x + _x, m_y + _y);
	}

	void MainSkin::move(int _x, int _y, int _cx, int _cy)
	{
		// присваеваем оверлею позицию отца плюс смещение внутри отца
		m_overlayContainer->setPosition(m_x + _x, m_y + _y);

		(m_parent->m_cx < 0) ? m_cx = 0 : m_cx = m_parent->m_cx;
		(m_parent->m_cy < 0) ? m_cy = 0 : m_cy = m_parent->m_cy;
		m_overlayContainer->setDimensions(m_cx, m_cy);
	}

	void MainSkin::align(int _cx, int _cy)
	{
		(m_parent->m_cx < 0) ? m_cx = 0 : m_cx = m_parent->m_cx;
		(m_parent->m_cy < 0) ? m_cy = 0 : m_cy = m_parent->m_cy;
		m_overlayContainer->setDimensions(m_cx, m_cy);
	}

	void MainSkin::correct()
	{
		// при нулевом смещении корректируем только один раз
		if (!m_parent->m_parent->m_left_margin && !m_parent->m_parent->m_top_margin) {
			if (m_correct) return;
			m_correct = true;
		} else m_correct = false;

//		debug.out("correct = %d, %d", m_parent->m_parent->m_left_margin, m_parent->m_parent->m_top_margin);
		m_overlayContainer->setPosition(m_x + m_parent->m_x - m_parent->m_parent->m_left_margin, m_y + m_parent->m_y - m_parent->m_parent->m_top_margin);
	}

	void MainSkin::restore()
	{
//		debug.out("restore");
		m_overlayContainer->setTransparent(false); // актуально для саб скина
		m_overlayContainer->setPosition(m_x + m_parent->m_x, m_y + m_parent->m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);
		// для восстановления нормально й позиции
		m_correct = false;
	}

	void MainSkin::update()
	{

		//порубали оверлей
		m_overlayContainer->setPosition(m_parent->view_left() - m_parent->m_parent->m_left_margin, m_parent->view_top() - m_parent->m_parent->m_top_margin);
		m_overlayContainer->setDimensions(m_parent->view_width(), m_parent->view_height());

		// теперь смещаем текстуру
		float UV_lft = m_parent->m_left_margin;
		float UV_top = m_parent->m_top_margin;
		float UV_rgt = m_parent->m_cx - m_parent->m_right_margin;
		float UV_btm = m_parent->m_cy - m_parent->m_bottom_margin;

		UV_lft = UV_lft / (float)m_parent->m_cx;
		UV_top = UV_top / (float)m_parent->m_cy;
		UV_rgt = UV_rgt / (float)m_parent->m_cx;
		UV_btm = UV_btm / (float)m_parent->m_cy;

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