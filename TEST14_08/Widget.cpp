
#include "Widget.h"
#include "debugOut.h"

namespace widget
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, char _align, Widget * _parent) :
		SubWidget(_x, _y, _cx, _cy, _align, _parent),
		m_visible(true)
	{
	}

	Widget::~Widget()
	{
		while (m_widgetChild.size() > 0)  {
			delete m_widgetChild[0];
			m_widgetChild.pop_back();
		};
	}

	Widget * Widget::createChild(int _x, int _y, int _cx, int _cy, char _align)
	{
		Widget * widget = new Widget(_x, _y, _cx, _cy, _align, this);
		m_widgetChild.push_back(widget);

		return widget;
	}

	void Widget::move(int _x, int _y)
	{

		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		// ������� ������� , ��� ��������� ���� �����������
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->move(_x, _y);

		update();
	}

	void Widget::move(int _x, int _y, int _cx, int _cy)
	{

		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		if ((m_cx < 0) || (m_cy < 0)) {
			visible(false);
			debug.out("--------");
			return;
		}

		// ������� ������� , ��� ��������� ���� �����������
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->move(_x, _y, _cx, _cy);

		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy);

		update();

	}

	void Widget::size(int _cx, int _cy)
	{
		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		if ((m_cx < 0) || (m_cy < 0)) {
			visible(false);
			debug.out("--------");
			return;
		}

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy);

		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy);

		update();

	}

	void Widget::update()
	{

		if (!m_parent) return;

		if (!m_show) return;

		//�������, ��� �������� �������
		bool margin = false;

		//������� �� ������
		if (left()   < m_parent->m_left_margin) {
			m_left_margin = m_parent->m_left_margin - left();
			margin = true;
		} else m_left_margin = 0;

		//������� �� �������
		if (right()  > m_parent->m_cx - m_parent->m_right_margin) {
			m_right_margin = right() - (m_parent->m_cx - m_parent->m_right_margin);
			margin = true;
		} else m_right_margin = 0;

		//������� �� �����
		if (top()    < m_parent->m_top_margin) {
			m_top_margin = m_parent->m_top_margin - top();
			margin = true;
		} else m_top_margin = 0;

		//������� �� ����
		if (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) {
			m_bottom_margin = bottom() - (m_parent->m_cy - m_parent->m_bottom_margin);
			margin = true;
		} else m_bottom_margin = 0;

		// ���� �� ������ ����, �� �������� ��� ��������
		if (margin) {

			// ������ ������ ������
			if (right()  < m_parent->m_left_margin ) {
				visible(false);
				m_margin = margin;
				return;
			}
			// ������ ������ �������
			if (left()   > m_parent->m_cx - m_parent->m_right_margin )  {
				visible(false);
				m_margin = margin;
				return;
			} 
			// ������ ������ �����
			if (bottom() < m_parent->m_top_margin  ) {
				visible(false);
				m_margin = margin;
				return;
			}
			// ������ ������ ����
			if (top()    > m_parent->m_cy - m_parent->m_bottom_margin ) {
				visible(false);
				m_margin = margin;
				return;
			}

		} else if (m_margin) {
			// �������, �� ������ �� ��������, �� ���� ��������, � ����� �������, ��������������� �����
			visible(true);

			// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
			for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();

			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
				// ��������������� �������� � ���� ����� ������������ ���������
				(*skin)->restore();
				// ���������� �����, ��� ����� �����
				(*skin)->correct();
			}

			m_margin = margin;
			return;
		}

		// ���� ���� ��� �����, �� �������
		visible(true);

		// ��������� ���� �����, ���� ������� ���� ����� ��� ��� �����
		if (margin || m_margin) {

			// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
			for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();

			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

		} else {
			// ���� ��� � ����� ��������, � �� ���, ������ ����� ���� ����������������� ���� �����
			// ������ ��� ��� ������� ���������� ���������� ���� ������������� � ���������� ���
			// ��� ������� �������� � correct() ��������� ������ ���� ���
			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

		}

		// ���������� ������� ���������
		m_margin = margin;

	}

	void Widget::addSubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material, char _align, bool _main)
	{
		if (_main) m_subSkinChild.push_back(new MainSkin(_x, _y, _cx, _cy, _leftUV, _topUV, _rightUV, _bottomUV, _material, _align, this));
		else m_subSkinChild.push_back(new SubSkin(_x, _y, _cx, _cy, _leftUV, _topUV, _rightUV, _bottomUV, _material, _align, this));
	}

	void Widget::attach(Ogre::OverlayElement * _element)
	{

		if (m_parent) {
			// ���� � ���� ���� ���� ���� �������, �� ��������� ���������
			assert(((Widget*)m_parent)->m_subSkinChild.size() > 0);
			((Widget*)m_parent)->m_subSkinChild[0]->attach(_element);
		} else {
			// � ���� ���, �� �������� ����
			OverlayManager &overlayManager = OverlayManager::getSingleton();
			Overlay * overlay;
			overlay = overlayManager.create("WidgetOverlay");
			overlay->setZOrder(200);
			overlay->show();
			overlay->add2D(static_cast<Ogre::OverlayContainer*>(_element));
		}
	}

	void Widget::visible(bool _visible)
	{
		if (m_visible == _visible) return;
		m_visible = _visible;

		// ���� ������ �������������, �� �� ����������
		if (_visible && !m_show) return;

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++)
			(*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		// ���� ����� �� ������� �� �� ����������
		if (_show && !m_visible) return;

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++)
			(*skin)->show(m_show);
	}

	void Widget::align(int _cx, int _cy)
	{
		if (!m_parent) return;

		bool need_move = false;
		bool need_size = false;
		int x = m_x;
		int y = m_y;
		int cx = m_cx;
		int cy = m_cy;

		// �������������� ������������ 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// �����������
				cx = m_cx + (m_parent->m_cx - _cx);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = m_x + (m_parent->m_cx - _cx);
				need_move = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (m_parent->m_cx - m_cx) / 2;
			need_move = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// �����������
				cy = m_cy + (m_parent->m_cy - _cy);
				need_size = true;
			} else {
				y = m_y + (m_parent->m_cy - _cy);
				need_move = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (m_parent->m_cy - m_cy) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // ������ ���� �� ������� ������������ � ����

	}

} // namespace Widget