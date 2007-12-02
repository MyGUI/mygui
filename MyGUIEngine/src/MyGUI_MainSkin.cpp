/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_MainSkin.h"
#include "utility.h"

namespace MyGUI
{

	MainSkin::MainSkin(const CroppedRectangleInfo& _info, const Ogre::String& _material, CroppedRectanglePtr _parent, size_t _id) : 
	CroppedRectangleBase(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent)
	{

		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

		mOverlayContainer = static_cast<PanelAlphaOverlayElement*>( overlayManager.createOverlayElement(
			"PanelAlpha", util::toString("MainSkin_", this)) );

		mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
		mOverlayContainer->setPosition(mParent->getLeft() + mLeft, mParent->getTop() + mTop);
		mOverlayContainer->setDimensions(mWidth, mHeight);
		if (false == _material.empty() && (_info.offset.getWidth() != 0)) mOverlayContainer->setMaterialName(_material);

		mParent->_attachChild(this, false);
	}

	MainSkin::~MainSkin()
	{
		if (mOverlayContainer == null) return;
		// с защитой от удаления после шутдауна рендера
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(mOverlayContainer);
	}

	void MainSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 color[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		mOverlayContainer->setColor(*(Ogre::uint32*)color);
	}

	void MainSkin::show()
	{
		if (mShow) return;
		mShow = true;
		mOverlayContainer->show();
	}

	void MainSkin::hide()
	{
		if (false == mShow) return;
		mShow = false;
		mOverlayContainer->hide();
	}

	void MainSkin::_setAlign(int _width, int _height, bool _update)
	{

		if (_update) {
			_updateView();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(mParent->getWidth() < 0) ? mWidth = 0 : mWidth = mParent->getWidth();
		(mParent->getHeight() < 0) ? mHeight = 0 : mHeight = mParent->getHeight();
		mOverlayContainer->setDimensions(mWidth, mHeight);

	}

	void MainSkin::_setAlign(int _left, int _top, int _width, int _height, bool _update)
	{

		mOverlayContainer->setPosition(mLeft + mParent->getLeft() - mParent->getParent()->getMarginLeft(), mTop + mParent->getTop() - mParent->getParent()->getMarginTop());

		if (_update) {
			_updateView();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(mParent->getWidth() < 0) ? mWidth = 0 : mWidth = mParent->getWidth();
		(mParent->getHeight() < 0) ? mHeight = 0 : mHeight = mParent->getHeight();
		mOverlayContainer->setDimensions(mWidth, mHeight);

	}

	void MainSkin::_correctView()
	{
		// либо просто двигаться, либо с учетом выравнивания отца
		if (mParent->getParent()) mOverlayContainer->setPosition(mLeft + mParent->getLeft() - mParent->getParent()->getMarginLeft() + mLeftMargin, mTop + mParent->getTop() - mParent->getParent()->getMarginTop() + mTopMargin);
		else mOverlayContainer->setPosition(mLeft + mParent->getLeft(), mTop + mParent->getTop());
	}

	void MainSkin::_updateView()
	{
		int cx = mParent->getViewWidth();
		if (cx < 0) cx = 0;
		int cy = mParent->getViewHeight();
		if (cy < 0) cy = 0;

		//порубали оверлей
		mOverlayContainer->setPosition(mParent->getViewLeft() - (mParent->getParent() ? mParent->getParent()->getMarginLeft() : 0), mParent->getViewTop() - (mParent->getParent() ? mParent->getParent()->getMarginTop() : 0) );
		mOverlayContainer->setDimensions(cx, cy);

		// теперь смещаем текстуру
		float UV_lft = mParent->getMarginLeft() / (float)mParent->getWidth();
		float UV_top = mParent->getMarginTop() / (float)mParent->getHeight();
		float UV_rgt = (mParent->getWidth() - mParent->getMarginRight()) / (float)mParent->getWidth();
		float UV_btm = (mParent->getHeight() - mParent->getMarginBottom()) / (float)mParent->getHeight();

		float UV_sizeX = mRectTexture.right - mRectTexture.left;
		float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

		float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
		float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
		float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
		float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

		mOverlayContainer->setUV(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

	}

	void MainSkin::_attachChild(CroppedRectanglePtr _basis, bool _child)
	{
		mOverlayContainer->addChild(_basis->_getOverlayElement());
	}

	Ogre::OverlayElement* MainSkin::_getOverlayElement()
	{
		return mOverlayContainer;
	}

	void MainSkin::_setUVSet(const FloatRect & _rect)
	{
		MYGUI_ASSERT(null != mOverlayContainer);
		mRectTexture = _rect;
		// если обрезаны, то просчитываем с учето обрезки
		if (mMargin) {

			float UV_lft = mParent->getMarginLeft() / (float)mParent->getWidth();
			float UV_top = mParent->getMarginTop() / (float)mParent->getHeight();
			float UV_rgt = (mParent->getWidth() - mParent->getMarginRight()) / (float)mParent->getWidth();
			float UV_btm = (mParent->getHeight() - mParent->getMarginBottom()) / (float)mParent->getHeight();

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


} // namespace MainSkin