/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_TextEdit.h"
#include "utility.h"

namespace MyGUI
{

	TextEdit::TextEdit(const CroppedRectangleInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id) :
		CroppedRectangleBase(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		mOverlayContainer = static_cast<TextEditOverlayElement *>(overlayManager.createOverlayElement(
			"TextEdit", util::toString("TextEdit_", this)) );

		mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);

		mOverlayContainer->setPosition(mLeft, mTop);
		mOverlayContainer->setDimensions(mWidth, mHeight);

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
		return mOverlayContainer->getCharHeight();
	}

	void TextEdit::_updateView()
	{
		bool margin = _checkMargin();

		// ������� ������, �.�. ���� ������ ���������
		int x = mLeft  - mParent->getMarginLeft();
		int y = mTop  - mParent->getMarginTop();

		mOverlayContainer->setPosition(x, y);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				mOverlayContainer->hide();
				// ���������� ������� ���������
				mMargin = margin;

				return;
			}
		}
		
		if ((mMargin) || (margin)) { // �� �������� ��� ���� ��������

			mOverlayContainer->setMargin(mLeftMargin, mTopMargin, mRightMargin, mBottomMargin);
			mOverlayContainer->setDimensions(mWidth, mHeight);

		}

		// ���������� ������� ���������
		mMargin = margin;
		// ���� ���� ��� �����, �� �������
		mOverlayContainer->show();

	}

	void TextEdit::_setAlign(int _left, int _top, int _width, int _height, bool _update)
	{
		_setAlign(_width, _height, _update);
	}

	void TextEdit::_setAlign(int _width, int _height, bool _update)
	{
		// ���������� �����������
		bool need_update = true;//_update;

		// �������������� ������������ 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// �����������
				mWidth = mWidth + (mParent->getWidth() - _width);
				need_update = true;
				mMargin = true; // ��� ��������� �������� ��� �������������
			} else {
				// ������� �� ������� ����
				mLeft = mLeft + (mParent->getWidth() - _width);
				need_update = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// ������������ �� ����������� ��� ����������
			mLeft = (mParent->getWidth() - mWidth) / 2;
			need_update = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// �����������
				mHeight = mHeight + (mParent->getHeight() - _height);
				need_update = true;
				mMargin = true; // ��� ��������� �������� ��� �������������
			} else {
				mTop = mTop + (mParent->getHeight() - _height);
				need_update = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// ������������ �� ��������� ��� ����������
			mTop = (mParent->getHeight() - mHeight) / 2;
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