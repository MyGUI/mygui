
#include "MainSkin.h"
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
		m_overlayContainer->setPosition(m_parent->m_x + m_x, m_parent->m_y + m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setMaterialName(_material);

		m_parent->attach(m_overlayContainer);

	}

	SubSkin::~SubSkin()
	{
	}

	void SubSkin::move(int _x, int _y)
	{
		debug.out("move 1");
		// присваеваем оверлею позицию отца плюс смещение внутри отца
		m_overlayContainer->setPosition(m_x + _x, m_y + _y);
	}

	void SubSkin::move(int _x, int _y, int _cx, int _cy)
	{
		debug.out("move 2");
		// присваеваем оверлею позицию отца плюс смещение внутри отца
		// надо подумать
//		m_overlayContainer->setPosition(m_x + _x, m_y + _y);

		align(_cx, _cy);
//		size(_cx, _cy);
	}

//	void SubSkin::size(int _cx, int _cy)
//	{
//		debug.out("size");
//		m_cx = _cx;
//		m_cy = _cy;
//		update();
//		m_overlayContainer->setDimensions(m_cx, m_cy);
//	}

	void SubSkin::align(int _cx, int _cy)
	{
		m_overlayContainer->setTransparent(true);

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

		m_x = x;
		m_y = y;
		m_cx = cx;
		m_cy = cy;


		if (need_move) {
			m_overlayContainer->setPosition(x + m_parent->m_x, y + m_parent->m_y);
		} else if (need_size) {
			m_overlayContainer->setDimensions(cx, cy);
		} else update();

		if (!((m_parent->view_width() < cx) || (m_parent->view_height() < cy))) m_overlayContainer->setTransparent(false);

		//		debug.out("align  move[%d](%d, %d, %d, %d)", need_move, x, y, cx, cy);

	}

	void SubSkin::correct()
	{
		// при нулевом смещении корректируем только один раз
		if (!m_parent->m_parent->m_left_margin && !m_parent->m_parent->m_top_margin) {
			if (m_correct) return;
			m_correct = true;
		} else m_correct = false;

		debug.out("correct = %d, %d", m_parent->m_parent->m_left_margin, m_parent->m_parent->m_top_margin);
		m_overlayContainer->setPosition(m_x + m_parent->m_x - m_parent->m_parent->m_left_margin, m_y + m_parent->m_y - m_parent->m_parent->m_top_margin);
	}

	void SubSkin::restore()
	{
		debug.out("restore");
		m_overlayContainer->setTransparent(false); // актуально для саб скина
		m_overlayContainer->setPosition(m_x + m_parent->m_x, m_y + m_parent->m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);
		m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);
		// для восстановления нормально й позиции
		m_correct = false;
	}

	void SubSkin::update()
	{
		debug.out("update");

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


		// а вот двигать надо всегда, если отец порезан, иначе видимые глюки
//		if (m_parent->m_margin) {
			m_overlayContainer->setPosition(
				m_parent->view_left() - m_parent->m_parent->m_left_margin + ((m_parent->m_left_margin > m_x) ? 0 : m_x - m_parent->m_left_margin),
				m_parent->view_top() - m_parent->m_parent->m_top_margin + ((m_parent->m_top_margin > m_y) ? 0 : m_y - m_parent->m_top_margin)
				);
//		}

				// если мы вообще ушли, то скрываем сабскин
		if (margin) {

			// сохраняем
			m_margin = margin;

			// совсем уехали налево
			if (right()  < m_parent->m_left_margin ) {
				m_overlayContainer->setTransparent(true);
				return;
			}
			// совсем уехали направо
			if (left()   > m_parent->m_cx - m_parent->m_right_margin )  {
				m_overlayContainer->setTransparent(true);
				return;
			} 
			// совсем уехали вверх
			if (bottom() < m_parent->m_top_margin  ) {
				m_overlayContainer->setTransparent(true);
				return;
			}
			// совсем уехали вниз
			if (top()    > m_parent->m_cy - m_parent->m_bottom_margin ) {
				m_overlayContainer->setTransparent(true);
				return;
			}

			// контейнер битый

			m_overlayContainer->setDimensions(view_width(), view_height());

			// теперь смещаем текстуру
			Real UV_lft = m_left_margin;
			Real UV_top = m_top_margin;
			Real UV_rgt = m_cx - m_right_margin;
			Real UV_btm = m_cy - m_bottom_margin;

			UV_lft = UV_lft / (float)m_cx;
			UV_top = UV_top / (float)m_cy;
			UV_rgt = UV_rgt / (float)m_cx;
			UV_btm = UV_btm / (float)m_cy;

			Real UV_sizeX = m_baseRightUV - m_baseLeftUV;
			Real UV_sizeY = m_baseBottomUV - m_baseTopUV;

			Real UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
			Real UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
			Real UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
			Real UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

			m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		} else {

			if (m_margin) {

				// контейнер не битый а был битый, восстанавливаем
				m_overlayContainer->setDimensions(view_width(), view_height());

				m_overlayContainer->setUV(m_baseLeftUV, m_baseTopUV, m_baseRightUV, m_baseBottomUV);

//			} else {

				// контейнет не бытый и был не битый, значит ничего не нужно
				// только подвинуть, если отец порезан
			}

		} // if (margin) {

		// делаем видимым
		m_overlayContainer->setTransparent(false);
		// запоминаем состояние
		m_margin = margin;
	}

	void SubSkin::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;
		_show ? m_overlayContainer->show() : m_overlayContainer->hide();
	};

	void SubSkin::attach(Ogre::OverlayElement * _element)
	{
		m_overlayContainer->addChild(_element);
	}

} // namespace SubSkin