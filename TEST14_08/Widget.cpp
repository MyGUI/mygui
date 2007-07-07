
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

		// а вот теперь запоминаем новые координаты
		m_x = _x;
		m_y = _y;

		// двигаем дочерей , все остальные сами подвинуться
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->move(_x, _y);

		update();
	}

	void Widget::move(int _x, int _y, int _cx, int _cy)
	{

		// а вот теперь запоминаем новые координаты
		m_x = _x;
		m_y = _y;

		// меняем координаты местами
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

		// двигаем дочерей , все остальные сами подвинуться
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->move(_x, _y, _cx, _cy);

		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy);

		update();

	}

	void Widget::size(int _cx, int _cy)
	{
		// меняем координаты местами
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

		// передаем старую координату , до вызова, текущая координата отца должна быть новой
		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->align(_cx, _cy);

		for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->align(_cx, _cy);

		update();

	}

	void Widget::update()
	{

		if (!m_parent) return;

		if (!m_show) return;

		//смотрим, как порубать оверлей
		bool margin = false;

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

		// если мы вообще ушли, то скрываем все сабскины
		if (margin) {

			// совсем уехали налево
			if (right()  < m_parent->m_left_margin ) {
				visible(false);
				m_margin = margin;
				return;
			}
			// совсем уехали направо
			if (left()   > m_parent->m_cx - m_parent->m_right_margin )  {
				visible(false);
				m_margin = margin;
				return;
			} 
			// совсем уехали вверх
			if (bottom() < m_parent->m_top_margin  ) {
				visible(false);
				m_margin = margin;
				return;
			}
			// совсем уехали вниз
			if (top()    > m_parent->m_cy - m_parent->m_bottom_margin ) {
				visible(false);
				m_margin = margin;
				return;
			}

		} else if (m_margin) {
			// опаньки, мы сейчас не обрезаны, но были обрезаны, к черту расчеты, восстанавливаем скины
			visible(true);

			// обновляем наших детей, а они уже решат обновлять ли своих детей
			for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();

			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) {
				// восстанавливаем текстуру и если нужно корректируем положение
				(*skin)->restore();
				// остаточный сдвиг, без этого глюки
				(*skin)->correct();
			}

			m_margin = margin;
			return;
		}

		// если скин был скрыт, то покажем
		visible(true);

		// обновляем всех детей, если вьюпорт стал битый или был битый
		if (margin || m_margin) {

			// обновляем наших детей, а они уже решат обновлять ли своих детей
			for (widgetIterator widget = m_widgetChild.begin(); widget != m_widgetChild.end(); widget++) (*widget)->update();

			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->update();

		} else {
			// отец был с битым вьпортом, а мы нет, значит нужно чуть подкорректировать наши скины
			// потому что при обрезке логическая координата отца передвигается а физическая нет
			// при нулевом смещении в correct() смещается только один раз
			for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++) (*skin)->correct();

		}

		// запоминаем текущее состояние
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
			// если у отца есть хоть один сабскин, то оформляем отцовство
			assert(((Widget*)m_parent)->m_subSkinChild.size() > 0);
			((Widget*)m_parent)->m_subSkinChild[0]->attach(_element);
		} else {
			// а если нет, то создадим отца
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

		// если скрыто пользователем, то не показываем
		if (_visible && !m_show) return;

		for (skinIterator skin = m_subSkinChild.begin(); skin != m_subSkinChild.end(); skin++)
			(*skin)->show(m_visible);
	}

	void Widget::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		// если вышло за границу то не показываем
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

		// первоначальное выравнивание 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// растягиваем
				cx = m_cx + (m_parent->m_cx - _cx);
				need_size = true;
			} else {
				// двигаем по правому краю
				x = m_x + (m_parent->m_cx - _cx);
				need_move = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			x = (m_parent->m_cx - m_cx) / 2;
			need_move = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// растягиваем
				cy = m_cy + (m_parent->m_cy - _cy);
				need_size = true;
			} else {
				y = m_y + (m_parent->m_cy - _cy);
				need_move = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			y = (m_parent->m_cy - m_cy) / 2;
			need_move = true;
		}

		if (need_move) {
			if (need_size) move(x, y, cx, cy);
			else move(x, y);
		} else if (need_size) {
			size(cx, cy);
		} else update(); // только если не вызвано передвижение и сайз

	}

} // namespace Widget