#include "Widget.h"

namespace widget
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, Widget * _parent)
		: m_parent (_parent),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
		m_real_x (_x),
		m_real_y (_y),
		m_real_cx (_cx),
		m_real_cy (_cy)
	{
		check();
	}

	Widget::~Widget()
	{
		while (m_widgetChild.size() > 0)  {
			delete m_widgetChild[0];
			m_widgetChild.pop_back();
		};
	}

	Widget * Widget::createChild(int _x, int _y, int _cx, int _cy)
	{
		Widget * widget = new Widget(_x, _y, _cx, _cy, this);
		m_widgetChild.push_back(widget);
		return widget;
	}

	void Widget::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		check();
	}

	void Widget::move(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;
		check();
	}

	void Widget::check()
	{
		if (!m_parent) return;
	}

} // namespace Widget