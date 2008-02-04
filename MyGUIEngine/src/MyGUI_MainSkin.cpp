/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_MainSkin.h"
#include "MyGUI_Utility.h"

namespace MyGUI
{

	MainSkin::MainSkin(const SubWidgetInfo& _info, const Ogre::String& _material, CroppedRectanglePtr _parent, size_t _id) : 
		CroppedRectangleInterface(_info.coord, _info.align, _parent)
	{

		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

		mOverlayContainer = static_cast<SharedPanelAlphaOverlayElement*>( overlayManager.createOverlayElement(
			"SharedPanelAlpha", utility::toString("MainSkin_", this)) );

		// устанавливаем колличество саб оверлеев
		mOverlayContainer->setCountSharedOverlay(1);

		//mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
		mOverlayContainer->setPositionInfo(mParent->getLeft() + mCoord.left, mParent->getTop() + mCoord.top, mCoord.width, mCoord.height, 0);
		if (false == _material.empty() && (_info.coord.width != 0)) mOverlayContainer->setMaterialName(_material);

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
		Ogre::uint8 colour[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		mOverlayContainer->setColour(*(Ogre::uint32*)colour);
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

	void MainSkin::_setAlign(const IntSize& _size, bool _update)
	{

		if (_update) {
			_updateView();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(mParent->getWidth() < 0) ? mCoord.width = 0 : mCoord.width = mParent->getWidth();
		(mParent->getHeight() < 0) ? mCoord.height = 0 : mCoord.height = mParent->getHeight();
		mOverlayContainer->setDimensionInfo(mCoord.width, mCoord.height, 0);

	}

	void MainSkin::_setAlign(const IntCoord& _coord, bool _update)
	{
		CroppedRectangleInterface * parent = mParent->getParent();
		int margin_left, margin_top;
		if (null != parent) {
			margin_left = parent->getMarginLeft();
			margin_top = parent->getMarginTop();
		}
		else {
			margin_left = 0;
			margin_top = 0;
		}

		mOverlayContainer->setPositionInfo(mCoord.left + mParent->getLeft() - margin_left, mCoord.top + mParent->getTop() - margin_top, 0);

		if (_update) {
			_updateView();
			return;
		}

		// если обновлять не надо, то меняем только размер
		(mParent->getWidth() < 0) ? mCoord.width = 0 : mCoord.width = mParent->getWidth();
		(mParent->getHeight() < 0) ? mCoord.height = 0 : mCoord.height = mParent->getHeight();
		mOverlayContainer->setDimensionInfo(mCoord.width, mCoord.height, 0);

	}

	void MainSkin::_correctView()
	{
		// либо просто двигаться, либо с учетом выравнивания отца
		if (mParent->getParent()) mOverlayContainer->setPositionInfo(mCoord.left + mParent->getLeft() - mParent->getParent()->getMarginLeft() + mMargin.left, mCoord.top + mParent->getTop() - mParent->getParent()->getMarginTop() + mMargin.top, 0);
		else mOverlayContainer->setPositionInfo(mCoord.left + mParent->getLeft(), mCoord.top + mParent->getTop(), 0);
	}

	void MainSkin::_updateView()
	{
		int cx = mParent->getViewWidth();
		if (cx < 0) cx = 0;
		int cy = mParent->getViewHeight();
		if (cy < 0) cy = 0;

		//порубали оверлей
		mOverlayContainer->setPositionInfo(mParent->getViewLeft() - (mParent->getParent() ? mParent->getParent()->getMarginLeft() : 0), mParent->getViewTop() - (mParent->getParent() ? mParent->getParent()->getMarginTop() : 0), cx, cy, 0);

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

		mOverlayContainer->setUVInfo(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total, 0);

	}

	void MainSkin::_attachChild(CroppedRectanglePtr _basis, bool _child)
	{
		mOverlayContainer->addChild(_basis->_getOverlayElement());
	}

	Ogre::OverlayElement* MainSkin::_getOverlayElement()
	{
		return mOverlayContainer;
	}

	void MainSkin::_setUVSet(const FloatRect& _rect)
	{
		MYGUI_ASSERT(null != mOverlayContainer, "overlay is not create");
		mRectTexture = _rect;
		// если обрезаны, то просчитываем с учето обрезки
		if (mIsMargin) {

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

			mOverlayContainer->setUVInfo(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total, 0);

		} else {
			// мы не обрезаны, базовые координаты
			mOverlayContainer->setUVInfo(mRectTexture.left, mRectTexture.top, mRectTexture.right, mRectTexture.bottom, 0);
		}
	}

	void MainSkin::_setMaterialName(const Ogre::String& _material)
	{
		MYGUI_ASSERT(null != mOverlayContainer, "overlay is not create");
		if (false == _material.empty()) mOverlayContainer->setMaterialName(_material);
	}


} // namespace MainSkin
