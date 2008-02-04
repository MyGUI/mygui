/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_SubSkin.h"
#include "MyGUI_Utility.h"

namespace MyGUI
{

	SubSkin::SubSkin(const SubWidgetInfo &_info, const Ogre::String & _material, CroppedRectanglePtr _parent, size_t _id) :
		CroppedRectangleInterface(_info.coord, _info.align, _parent),
		mId(_id),
		mTransparent(false)
	{

		// если мы первые, то создаем оверлей
		if (mId == 0) {
			Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

			mOverlayContainer = static_cast<SharedPanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
				"SharedPanelAlpha", utility::toString("SubSkin_", this)) );

			// устанавливаем колличество саб оверлеев
			mOverlayContainer->setCountSharedOverlay(_parent->_getCountSharedOverlay());

			//mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
			mOverlayContainer->setPositionInfo(mParent->getLeft() + mCoord.left, mParent->getTop() + mCoord.top, mCoord.width, mCoord.height, mId);
			if (false == _material.empty()) mOverlayContainer->setMaterialName(_material);

			mParent->_attachChild(this, false);
		}
		// мы должны пользоваться общим оверлеем
		else {
			mOverlayContainer = static_cast<SharedPanelAlphaOverlayElement*>(_parent->_getSharedOverlayElement());
		}

	}

	SubSkin::~SubSkin()
	{
		if ((mOverlayContainer == null) || (mId != 0)) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(mOverlayContainer);
	}

	void SubSkin::show()
	{
		if (mShow) return;
		mShow = true;

		mOverlayContainer->show();
	}

	void SubSkin::hide()
	{
		if (false == mShow) return;
		mShow = false;

		mOverlayContainer->hide();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 colour[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		mOverlayContainer->setColour(*(Ogre::uint32*)colour);
	}

	void SubSkin::_attachChild(CroppedRectanglePtr _basis, bool _child)
	{
		if (mId == 0) mOverlayContainer->addChild(_basis->_getOverlayElement());
	}

	Ogre::OverlayElement* SubSkin::_getOverlayElement()
	{
		// возвращаем, если только мы главные
		if (mId != 0) return null;
		return mOverlayContainer;
	}

	void SubSkin::_correctView()
	{
		// либо просто двигаться, либо с учетом выравнивания отца
		if (mParent->getParent()) mOverlayContainer->setPositionInfo(mCoord.left + mParent->getLeft() - mParent->getParent()->getMarginLeft() + mMargin.left, mCoord.top + mParent->getTop() - mParent->getParent()->getMarginTop() + mMargin.top, mId);
		else mOverlayContainer->setPositionInfo(mCoord.left + mParent->getLeft(), mCoord.top + mParent->getTop(), mId);
	}

	void SubSkin::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void SubSkin::_setAlign(const IntSize& _size, bool _update)
	{

		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (IS_ALIGN_RIGHT(mAlign)) {
			if (IS_ALIGN_LEFT(mAlign)) {
				// растягиваем
				mCoord.width = mCoord.width + (mParent->getWidth() - _size.width);
				need_update = true;
				mIsMargin = true; // при изменении размеров все пересчитывать
			}
			else {
				// двигаем по правому краю
				mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_update = true;
			}

		}
		else if (false == IS_ALIGN_LEFT(mAlign)) {
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (IS_ALIGN_BOTTOM(mAlign)) {
			if (IS_ALIGN_TOP(mAlign)) {
				// растягиваем
				mCoord.height = mCoord.height + (mParent->getHeight() - _size.height);
				need_update = true;
				mIsMargin = true; // при изменении размеров все пересчитывать
			}
			else {
				mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_update = true;
			}
		}
		else if (false == IS_ALIGN_TOP(mAlign)) {
			// выравнивание по вертикали без растяжения
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) _updateView();

	}

	void SubSkin::_updateView()
	{

		bool margin = _checkMargin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = mCoord.left + mParent->getLeft() - (mParent->getParent() ? mParent->getParent()->getMarginLeft() : 0) + mMargin.left;
		int y = mCoord.top + mParent->getTop() - (mParent->getParent() ? mParent->getParent()->getMarginTop() : 0) + mMargin.top;

		mOverlayContainer->setPositionInfo(x, y, mId);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (_checkOutside()) {

				// скрываем
				_setTransparent(true);
				// запоминаем текущее состояние
				mIsMargin = margin;

				return;

			}

		}

		if ((mIsMargin) || (margin)) { // мы обрезаны или были обрезаны

			int cx = getViewWidth();
			if (cx < 0) cx = 0;
			int cy = getViewHeight();
			if (cy < 0) cy = 0;

			mOverlayContainer->setDimensionInfo(cx, cy, mId);

			if (cx && cy) {

				// теперь смещаем текстуру
				float UV_lft = mMargin.left / (float)mCoord.width;
				float UV_top = mMargin.top / (float)mCoord.height;
				float UV_rgt = (mCoord.width - mMargin.right) / (float)mCoord.width;
				float UV_btm = (mCoord.height - mMargin.bottom) / (float)mCoord.height;

				float UV_sizeX = mRectTexture.right - mRectTexture.left;
				float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

				float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
				float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
				float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
				float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

				mOverlayContainer->setUVInfo(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total, mId);

			}

		}

		// запоминаем текущее состояние
		mIsMargin = margin;
		// если скин был скрыт, то покажем
		_setTransparent(false);

	}

	void SubSkin::_setUVSet(const FloatRect& _rect)
	{
		MYGUI_DEBUG_ASSERT(null != mOverlayContainer, "overlay is not create");
		mRectTexture = _rect;

		// если обрезаны, то просчитываем с учето обрезки
		if (mIsMargin) {

			float UV_lft = mMargin.left / (float)mCoord.width;
			float UV_top = mMargin.top / (float)mCoord.height;
			float UV_rgt = (mCoord.width - mMargin.right) / (float)mCoord.width;
			float UV_btm = (mCoord.height - mMargin.bottom) / (float)mCoord.height;

			float UV_sizeX = mRectTexture.right - mRectTexture.left;
			float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

			float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

			mOverlayContainer->setUVInfo(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total, mId);
		}

		// мы не обрезаны, базовые координаты
		else mOverlayContainer->setUVInfo(mRectTexture.left, mRectTexture.top, mRectTexture.right, mRectTexture.bottom, mId);

	}

} // namespace MyGUI
