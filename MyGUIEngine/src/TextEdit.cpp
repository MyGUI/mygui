
#include "TextEdit.h"
#include <OgreStringConverter.h>

namespace MyGUI
{

	TextEdit::TextEdit(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		m_overlayContainer = static_cast<TextEditOverlayElement *>(overlayManager.createOverlayElement(
			"TextEdit", "TextEdit_" + Ogre::StringConverter::toString((Ogre::uint32)this)));

		m_overlayContainer->setMetricsMode(Ogre::GMM_PIXELS);

		m_overlayContainer->setPosition(m_x, m_y);
		m_overlayContainer->setDimensions(m_cx, m_cy);

		mParent->attach(this, true);
	}

	TextEdit::~TextEdit()
	{
		if (m_overlayContainer == null) return;
		// � ������� �� �������� ����� �������� �������
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(m_overlayContainer);
	}

	void TextEdit::setTextAlign(Align _align)
	{
		// ������������
		m_overlayContainer->setAlignment(_align);
		updateText();
	}

	Ogre::OverlayElement* TextEdit::getOverlayElement()
	{
		return m_overlayContainer;
	}

	void TextEdit::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;

		mShow ? m_overlayContainer->show():m_overlayContainer->hide();
	}

	void TextEdit::setCaption(const Ogre::DisplayString & _caption)
	{
		// ��������� ����� ������
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

		// ������� ������, �.�. ���� ������ ���������
		int x = m_x  - mParent->margin_left();
		int y = m_y  - mParent->margin_top();

		m_overlayContainer->setPosition(x, y);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (check_outside()) {

				// ��������
				m_overlayContainer->hide();
				// ���������� ������� ���������
				mMargin = margin;

				return;
			}
		}
		
		if ((mMargin) || (margin)) { // �� �������� ��� ���� ��������

			m_overlayContainer->setMargin(m_left_margin, m_top_margin, m_right_margin, m_bottom_margin);
			m_overlayContainer->setDimensions(m_cx, m_cy);

		}

		// ���������� ������� ���������
		mMargin = margin;
		// ���� ���� ��� �����, �� �������
		m_overlayContainer->show();

	}

	void TextEdit::align(int _x, int _y, int _cx, int _cy, bool _update)
	{
		align(_cx, _cy, _update);
	}

	void TextEdit::align(int _cx, int _cy, bool _update)
	{
		// ���������� �����������
		bool need_update = true;//_update;

		// �������������� ������������ 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// �����������
				m_cx = m_cx + (mParent->width() - _cx);
				need_update = true;
				mMargin = true; // ��� ��������� �������� ��� �������������
			} else {
				// ������� �� ������� ����
				m_x = m_x + (mParent->width() - _cx);
				need_update = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			m_x = (mParent->width() - m_cx) / 2;
			need_update = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// �����������
				m_cy = m_cy + (mParent->height() - _cy);
				need_update = true;
				mMargin = true; // ��� ��������� �������� ��� �������������
			} else {
				m_y = m_y + (mParent->height() - _cy);
				need_update = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			m_y = (mParent->height() - m_cy) / 2;
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

	// ���������� ������ ������ � ��������
	IntSize TextEdit::getTextSize()
	{
		return m_overlayContainer->getTextSize();
	}
	// ������������� �������� ������ � ��������
	void TextEdit::setTextShift(IntPoint _point)
	{
		m_overlayContainer->setTextShift(_point);
	}
	IntPoint TextEdit::getTextShift()
	{
		return m_overlayContainer->getTextShift();
	}

} // namespace MyGUI