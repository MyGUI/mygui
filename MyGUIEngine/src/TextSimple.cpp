
#include "TextSimple.h"
#include "BasisWidgetManager.h"

#include <OgreStringConverter.h>

namespace MyGUI
{
	// ������� ������� ��� ����� �����
	BasisWidgetFactory<TextSimple> factory_TextSimple;

	TextSimple::TextSimple(const tagBasisWidgetInfo &_info, const String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.aligin, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<TextSimpleOverlayElement *>(overlayManager.createOverlayElement(
			"TextSimple", "TextSimple_" + Ogre::StringConverter::toString((uint32)this)));

		m_overlayContainer->setMetricsMode(GMM_PIXELS);

		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);

		m_parent->attach(this, true);
	}

	TextSimple::~TextSimple()
	{
		if (m_overlayContainer == null) return;
		// � ������� �� �������� ����� �������� �������
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(m_overlayContainer);
	}

	void TextSimple::setTextAlign(char _align)
	{
		// ������������
		m_overlayContainer->setAlignment(_align);
		updateText();
	}

	OverlayElementPtr TextSimple::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void TextSimple::show(bool _show)
	{
		if (m_show == _show) return;
		m_show = _show;

		m_show ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void TextSimple::setCaption(const Ogre::DisplayString & _caption)
	{
		// ��������� ����� ������
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

		// ������� ������, �.�. ���� ������ ���������
		int x = m_x  - m_parent->margin_left();
		int y = m_y  - m_parent->margin_top();

		m_overlayContainer->setPosition(x, y);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				m_overlayContainer->hide();
				// ���������� ������� ���������
				m_margin = margin;

				return;
			}
		}
		
		if ((m_margin) || (margin)) { // �� �������� ��� ���� ��������

			m_overlayContainer->setMargin(m_left_margin, m_top_margin, m_right_margin, m_bottom_margin);
			m_overlayContainer->setDimensions(m_cx, m_cy);

		}

		// ���������� ������� ���������
		m_margin = margin;
		// ���� ���� ��� �����, �� �������
		m_overlayContainer->show();

	}

	void TextSimple::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void TextSimple::align(int _cx, int _cy, bool _update)
	{
		// ���������� �����������
		bool need_update = true;//_update;

		// �������������� ������������ 
		if (m_align & ALIGN_RIGHT) {
			if (m_align & ALIGN_LEFT) {
				// �����������
				m_cx = m_cx + (m_parent->width() - _cx);
				need_update = true;
				m_margin = true; // ��� ��������� �������� ��� �������������
			} else {
				// ������� �� ������� ����
				m_x = m_x + (m_parent->width() - _cx);
				need_update = true;
			}

		} else if (!(m_align & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			m_x = (m_parent->width() - m_cx) / 2;
			need_update = true;
		}

		if (m_align & ALIGN_BOTTOM) {
			if (m_align & ALIGN_TOP) {
				// �����������
				m_cy = m_cy + (m_parent->height() - _cy);
				need_update = true;
				m_margin = true; // ��� ��������� �������� ��� �������������
			} else {
				m_y = m_y + (m_parent->height() - _cy);
				need_update = true;
			}
		} else if (!(m_align & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			m_y = (m_parent->height() - m_cy) / 2;
			need_update = true;
		}

		if (need_update) update();

	}

	void TextSimple::setTextSelect(size_t _start, size_t _end)
	{
		m_overlayContainer->setSelect(_start, _end);
	}

} // namespace MyGUI