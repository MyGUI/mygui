#pragma once

#include "SubWidget.h"

namespace widget
{

	SubWidget::SubWidget(int _x, int _y, int _cx, int _cy, char _align, SubWidget * _parent) :
		m_parent (_parent),
		m_align (_align),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
		m_left_margin (0),
		m_right_margin (0),
		m_top_margin (0),
		m_bottom_margin (0),
		m_margin(false),
		m_correct(false),
		m_show(true)
	{}

	SubWidget::~SubWidget() {}

	void SubWidget::move(int _x, int _y) {}
	void SubWidget::move(int _x, int _y, int _cx, int _cy) {}

	void SubWidget::update() {}
	void SubWidget::restore() {}
	void SubWidget::correct() {}

	void SubWidget::show(bool _show) {}

	void SubWidget::attach(Ogre::OverlayElement * _element) {};

	void SubWidget::align(int _cx, int _cy) {}

	void SubWidget::size(int _cx, int _cy) {}

} // namespace widget