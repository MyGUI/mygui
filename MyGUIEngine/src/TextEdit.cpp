
#include "TextEdit.h"
#include "BasisWidgetManager.h"

#include <OgreStringConverter.h>

namespace MyGUI
{
	// создаем фабрику для этого скина
	BasisWidgetFactory<TextEdit> factory_TextSimple;

	TextEdit::TextEdit(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.aligin, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<TextEditOverlayElement *>(overlayManager.createOverlayElement(
			"TextEdit", "TextEdit_" + Ogre::StringConverter::toString((uint32)this)));

		m_overlayContainer->setMetricsMode(GMM_PIXELS);

		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);

		m_parent->attach(this, true);
	}

	TextEdit::~TextEdit()
	{
		if (m_overlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(m_overlayContainer);
	}

	void TextEdit::setTextAlign(char _align)
	{
		// выравнивание
		m_overlayContainer->setAlignment(_align);
		updateText();
	}

	OverlayElementPtr TextEdit::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void TextEdit::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;

		m_show ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void TextEdit::setCaption(const Ogre::DisplayString & _caption)
	{
		// записывам новую строку
		m_overlayContainer->setCaption(_caption);
		updateText();
	}

	const Ogre::DisplayString & TextEdit::getCaption()
	{
		return m_overlayContainer->getCaption();
	}

	void TextEdit::setColour(const Ogre::ColourValue & _color)
	{
		m_color.r = _color.r;
		m_color.g = _color.g;
		m_color.b = _color.b;
		m_overlayContainer->setColour(m_color);
	}

	void TextEdit::setAlpha(float _alpha)
	{
		m_color.a = _alpha;
		m_overlayContainer->setColour(m_color);
	}

	void TextEdit::setFontName(const Ogre::String & _font)
	{
		m_overlayContainer->setFontName(_font);
		updateText();
	}

	void TextEdit::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		m_overlayContainer->setFontName(_font);
		m_overlayContainer->setCharHeight(_height);
		updateText();
	}

	const Ogre::String & TextEdit::getFontName()
	{
		return m_overlayContainer->getFontName();
	}

	void TextEdit::setFontHeight(Ogre::ushort _height)
	{
		m_overlayContainer->setCharHeight(_height);
		updateText();
	}

	Ogre::ushort TextEdit::getFontHeight()
	{
		return m_overlayContainer->getCharHeight();
	}

	void TextEdit::update()
	{
		bool margin = check_margin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = m_x  - m_parent->margin_left();
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

			m_overlayContainer->setMargin(m_left_margin, m_top_margin, m_right_margin, m_bottom_margin);
			m_overlayContainer->setDimensions(m_cx, m_cy);

		}

		// запоминаем текущее состояние
		m_margin = margin;
		// если скин был скрыт, то покажем
		m_overlayContainer->show();

	}

	void TextEdit::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void TextEdit::align(int _cx, int _cy, bool _update)
	{
		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// растягиваем
				m_cx = m_cx + (m_parent->width() - _cx);
				need_update = true;
				m_margin = true; // при изменении размеров все пересчитывать
			} else {
				// двигаем по правому краю
				m_x = m_x + (m_parent->width() - _cx);
				need_update = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			m_x = (m_parent->width() - m_cx) / 2;
			need_update = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// растягиваем
				m_cy = m_cy + (m_parent->height() - _cy);
				need_update = true;
				m_margin = true; // при изменении размеров все пересчитывать
			} else {
				m_y = m_y + (m_parent->height() - _cy);
				need_update = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			m_y = (m_parent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

	void TextEdit::setTextSelect(size_t _start, size_t _end)
	{
		m_overlayContainer->setSelect(_start, _end);
	}

	size_t TextEdit::getTextCursorFromPoint(IntPoint & _point)
	{
		return m_overlayContainer->getTextCursorFromPoint(_point);
	}

	IntPoint TextEdit::getTextCursorFromPosition(size_t _position)
	{
		return m_overlayContainer->getTextCursorFromPosition(_position);
	}

	// возвращает размер текста в пикселях
	IntSize TextEdit::getTextSize()
	{
		return m_overlayContainer->getTextSize();
	}
	// устанавливает смещение текста в пикселях
	void TextEdit::setTextShift(IntPoint _point)
	{
		m_overlayContainer->setTextShift(_point);
	}
	IntPoint TextEdit::getTextShift()
	{
		return m_overlayContainer->getTextShift();
	}

} // namespace MyGUI