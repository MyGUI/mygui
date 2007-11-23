
#include "TextSimple.h"
#include <OgreStringConverter.h>

namespace MyGUI
{

	TextSimple::TextSimple(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<TextSimpleOverlayElement *>(overlayManager.createOverlayElement(
			"TextSimple", "TextSimple_" + Ogre::StringConverter::toString((Ogre::uint32)this)));

		m_overlayContainer->setMetricsMode(Ogre::GMM_PIXELS);

		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);

		mParent->attach(this, true);
	}

	TextSimple::~TextSimple()
	{
		if (m_overlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(m_overlayContainer);
	}

	void TextSimple::setTextAlign(Align _align)
	{
		// выравнивание
		m_overlayContainer->setAlignment(_align);
		updateText();
	}

	Ogre::OverlayElement* TextSimple::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void TextSimple::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;

		mShow ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void TextSimple::setCaption(const Ogre::DisplayString & _caption)
	{
		// записывам новую строку
		m_overlayContainer->setCaption(_caption);
		updateText();
	}

	const Ogre::DisplayString & TextSimple::getCaption()
	{
		return m_overlayContainer->getCaption();
	}

	void TextSimple::setColour(const Ogre::ColourValue & _color)
	{
		m_color.r = _color.r;
		m_color.g = _color.g;
		m_color.b = _color.b;
		m_overlayContainer->setColour(m_color);
	}

	void TextSimple::setAlpha(float _alpha)
	{
		m_color.a = _alpha;
		m_overlayContainer->setColour(m_color);
	}

	void TextSimple::setFontName(const Ogre::String & _font)
	{
		m_overlayContainer->setFontName(_font);
		updateText();
	}

	void TextSimple::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		m_overlayContainer->setFontName(_font);
		m_overlayContainer->setCharHeight(_height);
		updateText();
	}

	const Ogre::String & TextSimple::getFontName()
	{
		return m_overlayContainer->getFontName();
	}

	void TextSimple::setFontHeight(Ogre::ushort _height)
	{
		m_overlayContainer->setCharHeight(_height);
		updateText();
	}

	Ogre::ushort TextSimple::getFontHeight()
	{
		return m_overlayContainer->getCharHeight();
	}

	void TextSimple::update()
	{
		bool margin = check_margin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = m_x  - mParent->margin_left();
		int y = m_y  - mParent->margin_top();

		m_overlayContainer->setPosition(x, y);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (check_outside()) {

				// скрываем
				m_overlayContainer->hide();
				// запоминаем текущее состояние
				mMargin = margin;

				return;
			}
		}
		
		if ((mMargin) || (margin)) { // мы обрезаны или были обрезаны

			m_overlayContainer->setMargin(m_left_margin, m_top_margin, m_right_margin, m_bottom_margin);
			m_overlayContainer->setDimensions(m_cx, m_cy);

		}

		// запоминаем текущее состояние
		mMargin = margin;
		// если скин был скрыт, то покажем
		m_overlayContainer->show();

	}

	void TextSimple::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void TextSimple::align(int _cx, int _cy, bool _update)
	{
		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// растягиваем
				m_cx = m_cx + (mParent->width() - _cx);
				need_update = true;
				mMargin = true; // при изменении размеров все пересчитывать
			} else {
				// двигаем по правому краю
				m_x = m_x + (mParent->width() - _cx);
				need_update = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			m_x = (mParent->width() - m_cx) / 2;
			need_update = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// растягиваем
				m_cy = m_cy + (mParent->height() - _cy);
				need_update = true;
				mMargin = true; // при изменении размеров все пересчитывать
			} else {
				m_y = m_y + (mParent->height() - _cy);
				need_update = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			m_y = (mParent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

} // namespace MyGUI