
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
		m_overlayContainer->setCharHeight(12);
		m_overlayContainer->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));
		m_overlayContainer->setCaption("test");

		assert(((Widget*)m_parent)->m_subSkinChild.size() > 0);
		((Widget*)m_parent)->m_subSkinChild[0]->attach(m_overlayContainer);

	}

	TextSimple::~TextSimple()
	{
	}

//	void TextSimple::attach(Ogre::OverlayElement * _element)
//	{
//		m_overlayContainer->addChild(_element);
//	}

	void TextSimple::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;

		m_show ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void TextSimple::update()
	{

		bool margin = check_margin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = m_x - m_parent->margin_left();
		if (x < 0) x = 0;
		int y = m_y  - m_parent->margin_top();
		if (y < 0) y = 0;

		m_overlayContainer->setPosition(x, y);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (check_outside()) {

				// скрываем
				m_overlayContainer->hide();
				// запоминаем текущее состояние
				m_margin = margin;

				return;

			}

		}
		
		if ((m_margin) || (margin)) { // мы обрезаны или были обрезаны

			int cx = view_width();
			if (cx < 0) cx = 0;
			int cy = view_height();
			if (cy < 0) cy = 0;

			m_overlayContainer->setDimensions(cx, cy);

			if (cx && cy) {

				// теперь смещаем текстуру
/*				float UV_lft = m_left_margin;
				float UV_top = m_top_margin;
				float UV_rgt = m_cx - m_right_margin;
				float UV_btm = m_cy - m_bottom_margin;

				UV_lft = UV_lft / (float)m_cx;
				UV_top = UV_top / (float)m_cy;
				UV_rgt = UV_rgt / (float)m_cx;
				UV_btm = UV_btm / (float)m_cy;

				float UV_sizeX = m_baseRightUV - m_baseLeftUV;
				float UV_sizeY = m_baseBottomUV - m_baseTopUV;

				float UV_lft_total = m_baseLeftUV + UV_lft * UV_sizeX;
				float UV_top_total = m_baseTopUV + UV_top * UV_sizeY;
				float UV_rgt_total = m_baseRightUV - (1-UV_rgt) * UV_sizeX;
				float UV_btm_total = m_baseBottomUV - (1-UV_btm) * UV_sizeY;

				m_overlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);*/

			}

		}

		// запоминаем текущее состояние
		m_margin = margin;
		// если скин был скрыт, то покажем
		m_overlayContainer->show();


	}

//	void TextSimple::correct()
//	{
//		m_overlayContainer->setPosition(m_x + m_left_margin, m_y + m_top_margin);
//	}

	void TextSimple::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void TextSimple::align(int _cx, int _cy, bool _update)
	{

		bool need_update = _update;

		// первоначальное выравнивание 
		if (m_align & ALIGN_RIGHT) {
			// двигаем по правому краю
			m_x = (m_parent->width() - m_cx);
			need_update = true;

		} else if (!(m_align & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			m_x = (m_parent->width() - m_cx) / 2;
			need_update = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			m_y = (m_parent->height() - m_cy);
			need_update = true;

		} else if (!(m_align & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			m_y = (m_parent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

	void TextSimple::setCaption(const Ogre::String & _caption)
	{
		// записывам новую строку
		m_overlayContainer->setCaption(_caption);
		// узнаем новый размер
		m_overlayContainer->getTextSize(m_cx, m_cy);
		debug.out("%d, %d", m_cx, m_cy);
		// и делаем полное обновление и выравнивание
		m_margin = true; // при изменении размеров все пересчитывать
		align(m_cx, m_cy, true);
	}

} // namespace widget