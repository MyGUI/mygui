
#include "TextSimple.h"
#include "Widget.h"
#include "debugOut.h"

namespace widget
{

	TextSimple::TextSimple(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, SubWidget * _parent) :
		SubWidget(_x, _y, _cx, _cy, _align, _parent),
		m_baseLeftUV (_leftUV),
		m_baseTopUV (_topUV),
		m_baseRightUV (_rightUV),
		m_baseBottomUV (_bottomUV)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<TextSimpleOverlayElement*>(overlayManager.createOverlayElement(
			"TextSimple", "Widget_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setFontName("MyGUI_font");
		m_overlayContainer->setCharHeight(20);
		m_overlayContainer->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));

		assert(((Widget*)m_parent)->m_subSkinChild.size() > 0);
		((Widget*)m_parent)->m_subSkinChild[0]->attach(m_overlayContainer);

	}

	TextSimple::~TextSimple()
	{
	}

	void TextSimple::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;

		m_show ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void TextSimple::update()
	{

		bool margin = check_margin();

		// ������� ������, �.�. ���� ������ ���������
		int x = m_x - m_parent->margin_left();
//		if (x < 0) x = 0;
		int y = m_y  - m_parent->margin_top();
//		if (y < 0) y = 0;

		m_overlayContainer->setPosition(x, y);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				m_overlayContainer->hide();
				// ���������� ������� ���������
				m_margin = margin;

				return;

			}

		}
		
		if ((m_margin) || (margin)) { // �� �������� ��� ���� ��������

//			int cx = view_width();
//			if (cx < 0) cx = 0;
//			int cy = view_height();
//			if (cy < 0) cy = 0;

			// ������������� ����� ��� ������� ������
			m_overlayContainer->setMargin(m_left_margin, m_top_margin, m_right_margin, m_bottom_margin);

			// ������������� ������
//			m_overlayContainer->setDimensions(m_cx, m_cy);

		}

		// ���������� ������� ���������
		m_margin = margin;
		// ���� ���� ��� �����, �� �������
		m_overlayContainer->show();

	}

	void TextSimple::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void TextSimple::align(int _cx, int _cy, bool _update)
	{

		bool need_update = _update;

		// �������������� ������������ 
		if (m_align & ALIGN_RIGHT) {
			// ������� �� ������� ����
			m_x = (m_parent->width() - m_cx);
			need_update = true;

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			m_x = (m_parent->width() - m_cx) / 2;
			need_update = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			m_y = (m_parent->height() - m_cy);
			need_update = true;

		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			m_y = (m_parent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

	void TextSimple::setCaption(const Ogre::String & _caption)
	{
		// ��������� ����� ������
		m_overlayContainer->setCaption(_caption);
		// ������ ����� ������
		m_overlayContainer->getTextSize(m_cx, m_cy);
		debug.out("%d, %d", m_cx, m_cy);
		// � ������ ������ ���������� � ������������
		m_margin = true; // ��� ��������� �������� ��� �������������
		align(m_cx, m_cy, true);
	}

} // namespace widget