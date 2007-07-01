
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
		m_bottom_margin (0),
		m_margin(false)
//		created(0)
	{
/*		UV_lft_base = 0.2;
		UV_rgt_base = 0.6;
		UV_top_base = 0.2;
		UV_btm_base = 0.8;*/
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

/*	void Widget::size(int _cx, int _cy)
	{
		m_cx = _cx;
		m_cy = _cy;
		update();
	}*/

	void Widget::move(int _x, int _y)
	{

		// двигаем дочерей , все остальные сами подвинуться
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->move(_x, _y);

		// а вот теперь запоминаем новые координаты
		m_x = _x;
		m_y = _y;

		check();
	}

	void Widget::check()
	{
		if (!m_parent) return;

		bool margin = false;
		//смотрим, как порубать оверлей

		//вылезли ли налево
		if (left()   < m_parent->m_left_margin) {
			m_left_margin = m_parent->m_left_margin - left();
			margin = true;
		} else m_left_margin = 0;

		//вылезли ли направо
		if (right()  > m_parent->m_cx - m_parent->m_right_margin) {
			m_right_margin = right() - (m_parent->m_cx - m_parent->m_right_margin);
			margin = true;
		} else m_right_margin = 0;

		//вылезли ли вверх
		if (top()    < m_parent->m_top_margin) {
			m_top_margin = m_parent->m_top_margin - top();
			margin = true;
		} else m_top_margin = 0;

		//вылезли ли вниз
		if (bottom() > m_parent->m_cy - m_parent->m_bottom_margin) {
			m_bottom_margin = bottom() - (m_parent->m_cy - m_parent->m_bottom_margin);
			margin = true;
		} else m_bottom_margin = 0;

		// обновляем всех детей, если вьюпорт стал битый или был битый
		if (margin || m_margin) {

			// обновляем наших детей, а они уже решат обновлять ли своих детей
			for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->check();

			// если саб скин один, то пусть пользуется нашим вьюпортом
			if (m_subSkinChild.size() == 0) m_subSkinChild[0]->updateMain();
			// если несколько, то пускай используют и свое смещение
			else {
				for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->updateSub();
			}

		}

		// запоминаем текущее состояние
		m_margin = margin;

	}

/*	void Widget::update()
	{
		check();
		std::vector<Widget*>::iterator iter;
		for(iter = m_widgetChild.begin(); iter != m_widgetChild.end(); iter++) (*iter)->update();

		std::vector<SubSkin*>::iterator iter2;
		for (iter2 = m_subSkinChild.begin(); iter2 != m_subSkinChild.end(); iter2++) (*iter2)->update();
	}*/


	void Widget::addSubSkin(int _x, int _y, int _cx, int _cy, float _leftUV, float _topUV, float _rightUV, float _bottomUV, const String & _material)
	{
		m_subSkinChild.push_back(new SubSkin(_x, _y, _cx, _cy, _leftUV, _topUV, _rightUV, _bottomUV, _material, this, m_parent?m_parent->m_subSkinChild[0]->m_overlayContainer:0));
	}

} // namespace Widget