#pragma once

#include "SabSkin.h"
#include <vector>

namespace widget
{
	class Widget
	{

	public:
		Widget(int _x, int _y, int _cx, int _cy, Widget * _parent);
		virtual ~Widget();

		Widget * createChild(int _x, int _y, int _cx, int _cy);

		void size(int _cx, int _cy);
		void move(int _x, int _y);

	protected:
		void check();

	protected:

		Widget * m_parent;
		std::vector<Widget*> m_widgetChild;

		int m_x, m_y, m_cx, m_cy;
		int m_view_x, m_view_y, m_view_cx, m_view_cy;

	}; // class Widget

} // namespace widget