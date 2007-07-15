
#include "SubSkin.h"
#include "debugOut.h"

namespace widget
{

	SubSkin::SubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, SubWidget * _parent) :
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

	SubSkin::~SubSkin()
	{
	}

	void SubSkin::attach(Ogre::OverlayElement * _element)
	{
		m_overlayContainer->addChild(_element);
	}

	void SubSkin::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;

		m_show ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void SubSkin::update()
	{

		bool margin = check_margin();

		// ������� ������, �.�. ���� ������ ���������
		int x = m_x + m_parent->left() - m_parent->getParent()->margin_left() + m_left_margin;
		if (x < 0) x = 0;
		int y = m_y + m_parent->top() - m_parent->getParent()->margin_top() + m_top_margin;
		if (y < 0) y = 0;

		m_overlayContainer->setPosition(x, y);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				m_overlayContainer->setTransparent(true);
				// ���������� ������� ���������
				m_margin = margin;

				return;

			}

		}
		
		if ((m_margin) || (margin)) { // �� �������� ��� ���� ��������

			int cx = view_width();
			if (cx < 0) cx = 0;
			int cy = view_height();
			if (cy < 0) cy = 0;

			m_overlayContainer->setDimensions(cx, cy);

			if (cx && cy) {

				// ������ ������� ��������
				float UV_lft = m_left_margin;
				float UV_top = m_top_margin;
				float UV_rgt = m_cx - m_right_margin;
				float UV_btm = m_cy - m_bottom_margin;

				UV_lft = UV_lft / (float)m_cx;
				UV_top = UV_top / (float)m_cy;
				UV_rgt = UV_rgt / (float)m_cx;
				UV_btm = UV_btm / (float)m_cy;

				float UV_sizeX = m_baseRightUV - m_baseLeftUV;
				float UV_sizeY = m_baseBottomUV - m_baseTopUV;

				float UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
				float UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
				float UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
				float UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

				m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

			}

		}

		// ���������� ������� ���������
		m_margin = margin;
		// ���� ���� ��� �����, �� �������
		m_overlayContainer->setTransparent(false);


	}

	void SubSkin::correct()
	{
		m_overlayContainer->setPosition(m_x + m_parent->left() - m_parent->getParent()->margin_left() + m_left_margin, m_y + m_parent->top() - m_parent->getParent()->margin_top() + m_top_margin);
	}

	void SubSkin::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void SubSkin::align(int _cx, int _cy, bool _update)
	{

		bool need_update = _update;

		// �������������� ������������ 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// �����������
				m_cx = m_cx + (m_parent->width() - _cx);
				need_update = true;
				m_margin = true; // ��� ��������� �������� ��� �������������
			} else {
				// ������� �� ������� ����
				m_x = m_x + (m_parent->width() - _cx);
				need_update = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			m_x = (m_parent->width() - m_cx) / 2;
			need_update = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// �����������
				m_cy = m_cy + (m_parent->height() - _cy);
				need_update = true;
				m_margin = true; // ��� ��������� �������� ��� �������������
			} else {
				m_y = m_y + (m_parent->height() - _cy);
				need_update = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			m_y = (m_parent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

} // namespace widget