
#include "TextSimple.h"
#include "Widget.h"
#include "debugOut.h"

namespace widget
{

	TextSimple::TextSimple(char _align, SubWidget * _parent) :
		SubWidget(0, 0, 0, 0, _align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<TextSimpleOverlayElement*>(overlayManager.createOverlayElement(
			"TextSimple", "Widget_" + Ogre::StringConverter::toString((uint32)this)) );

		m_overlayContainer->setMetricsMode(GMM_PIXELS);
		m_overlayContainer->setFontName("MyGUI_font");
		m_overlayContainer->setCharHeight(20);
		m_overlayContainer->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));

		// выравнивание
		if (_align & ALIGN_RIGHT) m_overlayContainer->setAlignment(Ogre::TextAreaOverlayElement::Right);
		else if (! (_align & ALIGN_RIGHT)) m_overlayContainer->setAlignment(Ogre::TextAreaOverlayElement::Center);

		assert(((Widget*)m_parent)->m_subSkinChild.size() > 0);
		((Widget*)m_parent)->m_subSkinChild[0]->attach(m_overlayContainer);

	}

	TextSimple::~TextSimple()
	{
	}

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
		int y = m_y  - m_parent->margin_top();

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

			// устанавливаем рамку для обрезки текста
			m_overlayContainer->setMargin(m_left_margin, m_top_margin, m_right_margin, m_bottom_margin);

		}

		// запоминаем текущее состояние
		m_margin = margin;
		// если скин был скрыт, то покажем
		m_overlayContainer->show();

	}

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

	void TextSimple::setCaption(const Ogre::DisplayString & _caption)
	{
		// записывам новую строку
		m_overlayContainer->setCaption(_caption);
		// узнаем новый размер, там же он и запоминается
		m_overlayContainer->getTextSize(m_cx, m_cy);
		// и делаем полное обновление и выравнивание
		m_margin = true; // при изменении размеров все пересчитывать
		align(m_cx, m_cy, true);
	}

	void TextSimple::setColour(float _red, float _green, float _blue)
	{
		m_color.r = _red;
		m_color.g = _green;
		m_color.b = _blue;
		m_overlayContainer->setColour(m_color);
	}

	void TextSimple::setAlpha(float _alpha)
	{
		m_color.a = _alpha;
		m_overlayContainer->setColour(m_color);
	}

//	void TextSimple::setUVSet(size_t _num)
//	{
//		if (_num != 0) m_overlayContainer->setColour(ColourValue(1.0, 1.0, 1.0, 1.0));
//		else m_overlayContainer->setColour(ColourValue(1.0, 1.0, 1.0, 0.5));
//	}

} // namespace widget