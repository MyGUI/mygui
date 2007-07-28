
#include "Widget.h"
#include "debugOut.h"
#include "TextSimple.h"

namespace widget
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, char _align, Widget * _parent) :
		SubWidget(_x, _y, _cx, _cy, _align, _parent),
		m_text(0),
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

	SubWidget *  Widget::addSubSkin(int _x, int _y, int _cx, int _cy, const String & _material, char _align, bool _main)
	{
		SubWidget * sub;
		if (_main) sub = new MainSkin(0, 0, m_cx, m_cy, _material, _align, this);
		else sub = new SubSkin(_x, _y, _cx, _cy, _material, _align, this);
		m_subSkinChild.push_back(sub);
		return sub;
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

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		// ���� ����� �� ������� �� �� ����������
		if (_show && !m_visible) return;

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->show(m_show);
	}

	void Widget::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		// ��� ������� ��������� � �  �� ���������
		align(_cx, _cy, _update);
	}

	void Widget::align(int _cx, int _cy, bool _update)
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
				cx = m_cx + (m_parent->width() - _cx);
				need_size = true;
			} else {
				// ������� �� ������� ����
				x = m_x + (m_parent->width() - _cx);
				need_move = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			x = (m_parent->width() - m_cx) / 2;
			need_move = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// �����������
				cy = m_cy + (m_parent->height() - _cy);
				need_size = true;
			} else {
				y = m_y + (m_parent->height() - _cy);
				need_move = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			y = (m_parent->height() - m_cy) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // ������ ���� �� ������� ������������ � ����

	}


	void Widget::move(int _x, int _y)
	{

		// � ��� ������ ���������� ����� ����������
		m_x = _x;
		m_y = _y;

		update();
	}

	void Widget::move(int _x, int _y, int _cx, int _cy)
	{

		if (!m_parent) return;

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

		bool show = true;

		// ��������� ������������
		bool margin = check_margin();

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(m_x, m_y, _cx, _cy, m_margin || margin);
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(m_x, m_y, _cx, _cy, m_margin || margin);

		// ���������� ������� ���������
		m_margin = margin;

	}

	void Widget::size(int _cx, int _cy)
	{

		if (!m_parent) return;

		// ������ ���������� �������
		int tmp = m_cx;
		m_cx = _cx;
		_cx = tmp;

		tmp = m_cy;
		m_cy = _cy;
		_cy = tmp;

		bool show = true;

		// ��������� ������������
		bool margin = check_margin();

		if (margin) {
			// �������� �� ������ ����� �� �������
			if (check_outside()) {
				// ��������
				show = false;
			}
		}

		visible(show);

		// �������� ������ ���������� , �� ������, ������� ���������� ���� ������ ���� �����
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy, m_margin || margin);
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy, m_margin || margin);

		// ���������� ������� ���������
		m_margin = margin;

	}

	void Widget::update()
	{

		if (!m_parent) return;

		bool margin = check_margin();

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				visible(false);
				// ���������� ������� ���������
				m_margin = margin;
				return;

			}

		} else if (!m_margin) { // �� �� �������� � ���� ����������

			// ���������� ������� ���������
			m_margin = margin;

			// ��� ��� ���� ����� ���������� ���� ��� ��������
			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

			// ��������
			visible(true);

			return;

		}

		// ���������� ������� ���������
		m_margin = margin;

		// ��������� ����� �����, � ��� ��� ����� ��������� �� ����� �����
		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

		// ���� ���� ��� �����, �� �������
		visible(true);

	}

	void Widget::setCaption(const Ogre::DisplayString & _caption)
	{
		if (!m_text) return;
		m_text->setCaption(_caption);
	}

	void Widget::addText(char _align)
	{
		m_text = new TextSimple(_align, this);
		m_subSkinChild.push_back(m_text);
	}

	void Widget::setUVSet(size_t _num)
	{
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->setUVSet(_num);
	}

} // namespace widget