/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_TextEdit.h"
#include "MyGUI_Utility.h"

namespace MyGUI
{

	TextEdit::TextEdit(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id) :
		SubWidgetTextInterface(_info.coord, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		mOverlayContainer = static_cast<TextEditOverlayElement *>(overlayManager.createOverlayElement(
			"TextEdit", utility::toString("TextEdit_", this)) );

		//mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);

		mOverlayContainer->setPosition(mCoord.left, mCoord.top);
		mOverlayContainer->setDimensions(mCoord.width, mCoord.height);

		mParent->_attachChild(this, true);
	}

	TextEdit::~TextEdit()
	{
		if (mOverlayContainer == null) return;
		// � ������� �� �������� ����� �������� �������
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(mOverlayContainer);
	}

	void TextEdit::setTextAlign(Align _align)
	{
		// ������������
		mOverlayContainer->setAlignment(_align);
		_updateText();
	}

	Ogre::OverlayElement* TextEdit::_getOverlayElement()
	{
		return mOverlayContainer;
	}

	void TextEdit::show()
	{
		if (mShow) return;
		mShow = true;
		mOverlayContainer->show();
	}

	void TextEdit::hide()
	{
		if (false == mShow) return;
		mShow = false;
		mOverlayContainer->hide();
	}

	void TextEdit::setCaption(const Ogre::DisplayString & _caption)
	{
		// ��������� ����� ������
		mOverlayContainer->setCaption(_caption);
		_updateText();
	}

	const Ogre::DisplayString & TextEdit::getCaption()
	{
		return mOverlayContainer->getCaption();
	}

	void TextEdit::setColour(const Ogre::ColourValue & _colour)
	{
		mColour.r = _colour.r;
		mColour.g = _colour.g;
		mColour.b = _colour.b;
		mOverlayContainer->setColour(mColour);
	}

	void TextEdit::setAlpha(float _alpha)
	{
		mColour.a = _alpha;
		mOverlayContainer->setColour(mColour);
	}

	void TextEdit::setFontName(const Ogre::String & _font)
	{
		mOverlayContainer->setFontName(_font);
		_updateText();
	}

	void TextEdit::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		mOverlayContainer->setFontName(_font);
		mOverlayContainer->setCharHeight(_height);
		_updateText();
	}

	const Ogre::String & TextEdit::getFontName()
	{
		return mOverlayContainer->getFontName();
	}

	void TextEdit::setFontHeight(Ogre::ushort _height)
	{
		mOverlayContainer->setCharHeight(_height);
		_updateText();
	}

	Ogre::ushort TextEdit::getFontHeight()
	{
		return (Ogre::ushort)mOverlayContainer->getCharHeight();
	}

	void TextEdit::_updateView()
	{
		bool margin = _checkMargin();

		// ������� ������, �.�. ���� ������ ���������
		int x = mCoord.left  - mParent->getMarginLeft();
		int y = mCoord.top  - mParent->getMarginTop();

		mOverlayContainer->setPosition(x, y);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				mOverlayContainer->hide();
				// ���������� ������� ���������
				mIsMargin = margin;

				return;
			}
		}

		if ((mIsMargin) || (margin)) { // �� �������� ��� ���� ��������

			mOverlayContainer->setMargin(mMargin);
			mOverlayContainer->setDimensions(mCoord.width, mCoord.height);

		}

		// ���������� ������� ���������
		mIsMargin = margin;
		// ���� ���� ��� �����, �� �������
		mOverlayContainer->show();

	}

	void TextEdit::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void TextEdit::_setAlign(const IntSize& _size, bool _update)
	{
		// ���������� �����������
		bool need_update = true;//_update;

		// �������������� ������������
		if (IS_ALIGN_RIGHT(mAlign)) {
			if (IS_ALIGN_LEFT(mAlign)) {
				// �����������
				mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_update = true;
				mIsMargin = true; // ��� ��������� �������� ��� �������������
			} else {
				// ������� �� ������� ����
				mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_update = true;
			}

		} else if (false == IS_ALIGN_LEFT(mAlign)) {
			// ������������ �� ����������� ��� ����������
			mCoord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (IS_ALIGN_BOTTOM(mAlign)) {
			if (IS_ALIGN_TOP(mAlign)) {
				// �����������
				mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_update = true;
				mIsMargin = true; // ��� ��������� �������� ��� �������������
			} else {
				mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_update = true;
			}
		} else if (false == IS_ALIGN_TOP(mAlign)) {
			// ������������ �� ��������� ��� ����������
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) _updateView();

	}

	void TextEdit::setTextSelect(size_t _start, size_t _end)
	{
		mOverlayContainer->setSelect(_start, _end);
	}

	size_t TextEdit::getTextCursorFromPoint(IntPoint & _point)
	{
		return mOverlayContainer->getTextCursorFromPoint(_point);
	}

	IntPoint TextEdit::getTextCursorFromPosition(size_t _position)
	{
		return mOverlayContainer->getTextCursorFromPosition(_position);
	}

	// ���������� ������ ������ � ��������
	IntSize TextEdit::getTextSize()
	{
		return mOverlayContainer->getTextSize();
	}
	IntSize TextEdit::getTextSize(const Ogre::DisplayString& _text)
	{
		return mOverlayContainer->getTextSize(_text);
	}

	// ������������� �������� ������ � ��������
	void TextEdit::setTextShift(IntPoint _point)
	{
		mOverlayContainer->setTextShift(_point);
	}

	IntPoint TextEdit::getTextShift()
	{
		return mOverlayContainer->getTextShift();
	}

	bool TextEdit::getSelectBackground()
	{
		return mOverlayContainer->getSelectBackground();
	}

	void TextEdit::setSelectBackground(bool _normal)
	{
		mOverlayContainer->setSelectBackground(_normal);
	}

} // namespace MyGUI
