/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_TextSimple.h"
#include "utility.h"

namespace MyGUI
{

	TextSimple::TextSimple(const CroppedRectangleInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id) :
		SubWidgetTextInterface(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		mOverlayContainer = static_cast<TextSimpleOverlayElement *>(overlayManager.createOverlayElement(
			"TextSimple", util::toString("TextSimple_", this)) );

		mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);

		mOverlayContainer->setPosition(mLeft, mTop);
		mOverlayContainer->setDimensions(mWidth, mHeight);

		mParent->_attachChild(this, true);
	}

	TextSimple::~TextSimple()
	{
		if (mOverlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(mOverlayContainer);
	}

	void TextSimple::setTextAlign(Align _align)
	{
		// выравнивание
		mOverlayContainer->setAlignment(_align);
		_updateText();
	}

	Ogre::OverlayElement* TextSimple::_getOverlayElement()
	{
		return mOverlayContainer;
	}

	void TextSimple::show()
	{
		if (mShow) return;
		mShow = true;
		mOverlayContainer->show();
	}

	void TextSimple::hide()
	{
		if (false == mShow) return;
		mShow = false;
		mOverlayContainer->hide();
	}

	void TextSimple::setCaption(const Ogre::DisplayString & _caption)
	{
		// записывам новую строку
		mOverlayContainer->setCaption(_caption);
		_updateText();
	}

	const Ogre::DisplayString & TextSimple::getCaption()
	{
		return mOverlayContainer->getCaption();
	}

	void TextSimple::setColour(const Ogre::ColourValue & _colour)
	{
		mColour.r = _colour.r;
		mColour.g = _colour.g;
		mColour.b = _colour.b;
		mOverlayContainer->setColour(mColour);
	}

	void TextSimple::setAlpha(float _alpha)
	{
		mColour.a = _alpha;
		mOverlayContainer->setColour(mColour);
	}

	void TextSimple::setFontName(const Ogre::String & _font)
	{
		mOverlayContainer->setFontName(_font);
		_updateText();
	}

	void TextSimple::setFontName(const Ogre::String & _font, Ogre::ushort _height)
	{
		mOverlayContainer->setFontName(_font);
		mOverlayContainer->setCharHeight(_height);
		_updateText();
	}

	const Ogre::String & TextSimple::getFontName()
	{
		return mOverlayContainer->getFontName();
	}

	void TextSimple::setFontHeight(Ogre::ushort _height)
	{
		mOverlayContainer->setCharHeight(_height);
		_updateText();
	}

	Ogre::ushort TextSimple::getFontHeight()
	{
		return mOverlayContainer->getCharHeight();
	}

	void TextSimple::_updateView()
	{
		bool margin = _checkMargin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = mLeft  - mParent->getMarginLeft();
		int y = mTop  - mParent->getMarginTop();

		mOverlayContainer->setPosition(x, y);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				mOverlayContainer->hide();
				// запоминаем текущее состояние
				mMargin = margin;

				return;
			}
		}
		
		if ((mMargin) || (margin)) { // мы обрезаны или были обрезаны

			mOverlayContainer->setMargin(mLeftMargin, mTopMargin, mRightMargin, mBottomMargin);
			mOverlayContainer->setDimensions(mWidth, mHeight);

		}

		// запоминаем текущее состояние
		mMargin = margin;
		// если скин был скрыт, то покажем
		mOverlayContainer->show();

	}

	void TextSimple::_setAlign(int _left, int _top, int _width, int _height, bool _update)
	{
		_setAlign(_width, _height, _update);
	}

	void TextSimple::_setAlign(int _width, int _height, bool _update)
	{
		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание 
		if (mAlign & ALIGN_RIGHT) {
			if (mAlign & ALIGN_LEFT) {
				// растягиваем
				mWidth = mWidth + (mParent->getWidth() - _width);
				need_update = true;
				mMargin = true; // при изменении размеров все пересчитывать
			} else {
				// двигаем по правому краю
				mLeft = mLeft + (mParent->getWidth() - _width);
				need_update = true;
			}

		} else if (!(mAlign & ALIGN_LEFT)) {
			// выравнивание по горизонтали без растяжения
			mLeft = (mParent->getWidth() - mWidth) / 2;
			need_update = true;
		}

		if (mAlign & ALIGN_BOTTOM) {
			if (mAlign & ALIGN_TOP) {
				// растягиваем
				mHeight = mHeight + (mParent->getHeight() - _height);
				need_update = true;
				mMargin = true; // при изменении размеров все пересчитывать
			} else {
				mTop = mTop + (mParent->getHeight() - _height);
				need_update = true;
			}
		} else if (!(mAlign & ALIGN_TOP)) {
			// выравнивание по вертикали без растяжения
			mTop = (mParent->getHeight() - mHeight) / 2;
			need_update = true;
		}

		if (need_update) _updateView();

	}

} // namespace MyGUI