/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_SubSkin.h"
#include "utility.h"

namespace MyGUI
{

	SubSkin::SubSkin(const BasisWidgetInfo &_info, const Ogre::String & _material, BasisWidgetPtr _parent, size_t _id) :
		BasisWidget(_info.offset.left, _info.offset.top, _info.offset.right, _info.offset.bottom, _info.align, _parent),
		mId(_id),
		mTransparent(false)
	{

		// ���� �� ������, �� ������� �������
		if (mId == 0) {
			Ogre::OverlayManager &overlayManager = Ogre::OverlayManager::getSingleton();

			mOverlayContainer = static_cast<SharedPanelAlphaOverlayElement*>(overlayManager.createOverlayElement(
				"SharedPanelAlpha", util::toString("SubSkin_", this)) );

			// ������������� ����������� ��� ��������
			mOverlayContainer->setCountSharedOverlay(_parent->_getCountSharedOverlay());

			mOverlayContainer->setMetricsMode(Ogre::GMM_PIXELS);
			mOverlayContainer->setPositionInfo(mParent->getLeft() + mLeft, mParent->getTop() + mTop, mWidth, mHeight, mId);
			if (false == _material.empty()) mOverlayContainer->setMaterialName(_material);

			mParent->_attachChild(this, false);
		}
		// �� ������ ������������ ����� ��������
		else {
			mOverlayContainer = static_cast<SharedPanelAlphaOverlayElement*>(_parent->_getSharedOverlayElement());
		}

	}

	SubSkin::~SubSkin()
	{
		if ((mOverlayContainer == null) || (mId != 0)) return;
		// � ������� �� �������� ����� �������� �������
		Ogre::OverlayManager * manager = Ogre::OverlayManager::getSingletonPtr();
		if (manager != null) manager->destroyOverlayElement(mOverlayContainer);
	}

	void SubSkin::show()
	{
		if (mShow) return;
		mShow = true;
		if (mTransparent) return;
//		mOverlayContainer->setTransparentInfo(false, mId);
		mOverlayContainer->show();
	}

	void SubSkin::hide()
	{
		if (false == mShow) return;
		mShow = false;
		if (mTransparent) return;
//		mOverlayContainer->setTransparentInfo(true, mId);
		mOverlayContainer->hide();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		Ogre::uint8 color[4] = {255, 255, 255, (Ogre::uint8)(_alpha*255)};
		mOverlayContainer->setColor(*(Ogre::uint32*)color);
	}

	void SubSkin::_attachChild(BasisWidgetPtr _basis, bool _child)
	{
		if (mId == 0) mOverlayContainer->addChild(_basis->_getOverlayElement());
	}

	Ogre::OverlayElement* SubSkin::_getOverlayElement()
	{
		// ����������, ���� ������ �� �������
		if (mId != 0) return null;
		return mOverlayContainer;
	}

	void SubSkin::_correctView()
	{
		// ���� ������ ���������, ���� � ������ ������������ ����
		if (mParent->getParent()) mOverlayContainer->setPositionInfo(mLeft + mParent->getLeft() - mParent->getParent()->getMarginLeft() + mLeftMargin, mTop + mParent->getTop() - mParent->getParent()->getMarginTop() + mTopMargin, mId);
		else mOverlayContainer->setPositionInfo(mLeft + mParent->getLeft(), mTop + mParent->getTop(), mId);
	}

	void SubSkin::_setAlign(int _left, int _top, int _width, int _height, bool _update)
	{
		_setAlign(_width, _height, _update);
	}

	void SubSkin::_setAlign(int _width, int _height, bool _update)
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

	void SubSkin::_updateView()
	{

		bool margin = _checkMargin();

		// ������� ������, �.�. ���� ������ ���������
		int x = mLeft + mParent->getLeft() - (mParent->getParent() ? mParent->getParent()->getMarginLeft() : 0) + mLeftMargin;
		int y = mTop + mParent->getTop() - (mParent->getParent() ? mParent->getParent()->getMarginTop() : 0) + mTopMargin;

		mOverlayContainer->setPositionInfo(x, y, mId);

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				_setTransparent(true);
				// ���������� ������� ���������
				mMargin = margin;

				return;

			}

		}
		
		if ((mMargin) || (margin)) { // �� �������� ��� ���� ��������

			int cx = getViewWidth();
			if (cx < 0) cx = 0;
			int cy = getViewHeight();
			if (cy < 0) cy = 0;

			mOverlayContainer->setDimensionInfo(cx, cy, mId);

			if (cx && cy) {

				// ������ ������� ��������
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

				mOverlayContainer->setUVInfo(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total, mId);

			}

		}

		// ���������� ������� ���������
		mMargin = margin;
		// ���� ���� ��� �����, �� �������
		_setTransparent(false);

	}

	void SubSkin::_setUVSet(const FloatRect & _rect)
	{
		MYGUI_DEBUG_ASSERT(null != mOverlayContainer);
		mRectTexture = _rect;

		// ���� ��������, �� ������������ � ����� �������
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

			mOverlayContainer->setUVInfo(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total, mId);
		}

		// �� �� ��������, ������� ����������
		else mOverlayContainer->setUVInfo(mRectTexture.left, mRectTexture.top, mRectTexture.right, mRectTexture.bottom, mId);

	}

} // namespace MyGUI