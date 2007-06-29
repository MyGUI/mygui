
#include "Widget.h"
//#include "MyGUI_Source//MyGUI_AssetManager.h"

namespace widget
{

	Widget::Widget(int _x, int _y, int _cx, int _cy, Widget * _parent)
		: m_parent (_parent),
		m_x (_x),
		m_y (_y),
		m_cx (_cx),
		m_cy (_cy),
		m_left_margin (0),
		m_right_margin (0),
		m_top_margin (0),
		m_bottom_margin (0)
//		created(0)
	{
/*		UV_lft_base = 0.2;
		UV_rgt_base = 0.6;
		UV_top_base = 0.2;
		UV_btm_base = 0.8;*/
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
		update();
		return widget;
	}

	void Widget::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		update();
	}

	void Widget::move(int _x, int _y)
	{
		m_x = _x;
		m_y = _y;
//		aligin();
		update();
	}

	void Widget::check()
	{
		if (!m_parent) return;

		//смотрим, как порубать оверлей
		m_left_margin   = (left()   < m_parent->m_left_margin) ?
									m_parent->m_left_margin - left() : 0; //вылезли ли налево
		m_right_margin  = (right()  > m_parent->m_cx - m_parent->m_right_margin) ?
				    right() - (m_parent->m_cx - m_parent->m_right_margin) : 0; //вылезли ли направо
		m_top_margin    = (top()    < m_parent->m_top_margin) ?
									  m_parent->m_top_margin - top() : 0; //вылезли ли вверх
		m_bottom_margin = (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) ?
				  bottom() - (m_parent->m_cy - m_parent->m_bottom_margin) : 0; //вылезли ли вниз


	}

	void Widget::update()
	{
		check();
		std::vector<Widget*>::iterator iter;
		for(iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) (*iter)->update();
		std::vector<SubSkin*>::iterator iter2;
		for (iter2 = m_subSkinChild.begin(); iter2 != m_subSkinChild.end(); iter2++) (*iter2)->update();
	}


	void Widget::addSubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material)
	{
		m_subSkinChild.push_back(new SubSkin(_x, _y, _cx, _cy, _leftUV, _topUV, _rightUV, _bottomUV, _material, this, m_parent?m_parent->m_subSkinChild[0]->m_overlayContainer:0));
	}

} // namespace Widget