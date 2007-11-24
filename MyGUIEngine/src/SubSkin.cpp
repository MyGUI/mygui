
#include "SubSkin.h"

namespace MyGUI
{

	SubSkin::SubSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{
		Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

		mOverlayContainer = static_cast<PanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
			"PanelAlpha", "SubSkin_" + Ogre::StringConverter::toString((Ogre::uint32)this)) );

		mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
		mOverlayContainer->setPosition(mParent->getLeft() + mLeft, mParent->getTop() + mTop);
		mOverlayContainer->setDimensions(mWidth, mHeight);
		if (false == _material.empty()) mOverlayContainer->setMaterialName(_material);

		mParent->attach(this, false);
	}

	SubSkin::~SubSkin()
	{
		if (mOverlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(mOverlayContainer);
	}

	void SubSkin::show(bool _show)
	{
		if (mShow == _show) return;
		mShow = _show;
		mShow ? mOverlayContainer->show():mOverlayContainer->hide();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 color[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		mOverlayContainer->setColor(*(Ogre::uint32*)color);
	}

	void SubSkin::attach(BasisWidgetPtr _basis, bool _child)
	{
		mOverlayContainer->addChild(_basis->getOverlayElement());
	}

	Ogre::OverlayElement* SubSkin::getOverlayElement()
	{
		return mOverlayContainer;
	}

	void SubSkin::correct()
	{
		// либо просто двигаться, либо с учетом выравнивания отца
		if (mParent->getParent()) mOverlayContainer->setPosition(mLeft + mParent->getLeft() - mParent->getParent()->getMarginLeft() + mLeftMargin, mTop + mParent->getTop() - mParent->getParent()->getMarginTop() + mTopMargin);
		else mOverlayContainer->setPosition(mLeft + mParent->getLeft(), mTop + mParent->getTop());
	}

	void SubSkin::align(int _left, int _top, int _width, int _height, bool _update)
	{
		align(_width, _height, _update);
	}

	void SubSkin::align(int _width, int _height, bool _update)
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

		if (need_update) update();

	}

	void SubSkin::update()
	{

		bool margin = checkMargin();

		// двигаем всегда, т.к. дети должны двигаться
		int x = mLeft + mParent->getLeft() - (mParent->getParent() ? mParent->getParent()->getMarginLeft() : 0) + mLeftMargin;
//		if (x < 0) x = 0;
		int y = mTop + mParent->getTop() - (mParent->getParent() ? mParent->getParent()->getMarginTop() : 0) + mTopMargin;
//		if (y < 0) y = 0;

		mOverlayContainer->setPosition(x, y);

		// вьюпорт стал битым
		if (margin) {

			// проверка на полный выход за границу
			if (checkOutside()) {

				// скрываем
				mOverlayContainer->setTransparent(true);
				// запоминаем текущее состояние
				mMargin = margin;

				return;

			}

		}
		
		if ((mMargin) || (margin)) { // мы обрезаны или были обрезаны

			int cx = getViewWidth();
			if (cx < 0) cx = 0;
			int cy = view_height();
			if (cy < 0) cy = 0;

			mOverlayContainer->setDimensions(cx, cy);

			if (cx && cy) {

				// теперь смещаем текстуру
				float UV_lft = mLeftMargin / (float)mWidth;
				float UV_top = mTopMargin / (float)mHeight;
				float UV_rgt = (mWidth - mRightMargin) / (float)mWidth;
				float UV_btm = (mHeight - mBottomMargin) / (float)mHeight;

				float UV_sizeX = mRectTexture.right - mRectTexture.left;
				float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

				float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
				float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
				float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
				float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

				mOverlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

			}

		}

		// запоминаем текущее состояние
		mMargin = margin;
		// если скин был скрыт, то покажем
		mOverlayContainer->setTransparent(false);

	}

	void SubSkin::setUVSet(const FloatRect & _rect)
	{
		assert(mOverlayContainer);
		mRectTexture = _rect;
		// если обрезаны, то просчитываем с учето обрезки
		if (mMargin) {

			float UV_lft = mLeftMargin / (float)mWidth;
			float UV_top = mTopMargin / (float)mHeight;
			float UV_rgt = (mWidth - mRightMargin) / (float)mWidth;
			float UV_btm = (mHeight - mBottomMargin) / (float)mHeight;

			float UV_sizeX = mRectTexture.right - mRectTexture.left;
			float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

			float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

			mOverlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		} else {
			// мы не обрезаны, базовые координаты
			mOverlayContainer->setUV(mRectTexture.left, mRectTexture.top, mRectTexture.right, mRectTexture.bottom);
		}
	}

} // namespace MyGUI