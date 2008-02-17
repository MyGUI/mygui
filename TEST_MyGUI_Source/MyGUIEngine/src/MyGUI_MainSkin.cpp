/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_MainSkin.h"
#include "MyGUI_RenderItem.h"

namespace MyGUI
{

	const size_t MAINSKIN_COUNT_VERTEX = VERTEX_IN_QUAD;

	MainSkin::MainSkin(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		CroppedRectangleInterface(_info.coord, _info.align, _parent),
		mTransparent(false),
		mRenderItem(null),
		mCurrentCoord(_info.coord),
		mCurrentAlpha(0xFFFFFFFF)
	{
	}

	MainSkin::~MainSkin()
	{
	}

	void MainSkin::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_correctView()
	{
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_updateView()
	{

		if (mParent->isMargin()) {

			if ((mParent->getViewWidth() <= 0) || (mParent->getViewHeight() <= 0)) {
				// скрываем
				if (false == mTransparent) mTransparent = true;
				mIsMargin = true;

				// обновить перед выходом
				if (null != mRenderItem) mRenderItem->outOfDate();
				return;
			}

			// теперь смещаем текстуру
			float UV_lft = (float)mParent->getMarginLeft() / (float)mParent->getWidth();
			float UV_top = (float)mParent->getMarginTop() / (float)mParent->getHeight();
			float UV_rgt = (float)(mParent->getWidth() - mParent->getMarginRight()) / (float)mParent->getWidth();
			float UV_btm = (float)(mParent->getHeight() - mParent->getMarginBottom()) / (float)mParent->getHeight();

			float UV_sizeX = mRectTexture.right - mRectTexture.left;
			float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

			float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

			mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		}
		else if (mIsMargin) {
			// мы не обрезаны, базовые координаты
			mCurrentTexture = mRectTexture;
		}

		// запоминаем текущее состояние
		mIsMargin = mParent->isMargin();

		// если скин был скрыт, то покажем
		if (mTransparent) mTransparent = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_setUVSet(const FloatRect& _rect)
	{
		mRectTexture = _rect;

		// если обрезаны, то просчитываем с учето обрезки
		if (mParent->isMargin()) {

			float UV_lft = (float)mParent->getMarginLeft() / (float)mParent->getWidth();
			float UV_top = (float)mParent->getMarginTop() / (float)mParent->getHeight();
			float UV_rgt = (float)(mParent->getWidth() - mParent->getMarginRight()) / (float)mParent->getWidth();
			float UV_btm = (float)(mParent->getHeight() - mParent->getMarginBottom()) / (float)mParent->getHeight();

			float UV_sizeX = mRectTexture.right - mRectTexture.left;
			float UV_sizeY = mRectTexture.bottom - mRectTexture.top;

			float UV_lft_total = mRectTexture.left + UV_lft * UV_sizeX;
			float UV_top_total = mRectTexture.top + UV_top * UV_sizeY;
			float UV_rgt_total = mRectTexture.right - (1-UV_rgt) * UV_sizeX;
			float UV_btm_total = mRectTexture.bottom - (1-UV_btm) * UV_sizeY;

			mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		}
		else {
			// мы не обрезаны, базовые координаты
			mCurrentTexture = mRectTexture;

		}

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void MainSkin::_drawItem(Vertex * _vertex, size_t & _count)
	{
		if ((false == mShow) || (mTransparent)) return;
		if ((0 >= mCurrentCoord.width) || (0 >= mCurrentCoord.height)) return;

		float vertex_z = mRenderItem->getMaximumDepth();

		float vertex_left = ((mRenderItem->getPixScaleX() * (float)(mParent->getMarginLeft() + mParent->getAbsoluteLeft()) + mRenderItem->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mRenderItem->getPixScaleX() * (float)mParent->getViewWidth() * 2);
		float vertex_top = -(((mRenderItem->getPixScaleY() * (float)(mParent->getMarginTop() + mParent->getAbsoluteTop()) + mRenderItem->getVOffset()) * 2) - 1);
		float vertex_bottom = vertex_top - (mRenderItem->getPixScaleY() * (float)mParent->getViewHeight() * 2);

		// first triangle - left top
		_vertex[0].x = vertex_left;
		_vertex[0].y = vertex_top;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mCurrentAlpha;
		_vertex[0].u = mCurrentTexture.left;
		_vertex[0].v = mCurrentTexture.top;
		

		// first triangle - left bottom
		_vertex[1].x = vertex_left;
		_vertex[1].y = vertex_bottom;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mCurrentAlpha;
		_vertex[1].u = mCurrentTexture.left;
		_vertex[1].v = mCurrentTexture.bottom;

		// first triangle - right top
		_vertex[2].x = vertex_right;
		_vertex[2].y = vertex_top;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mCurrentAlpha;
		_vertex[2].u = mCurrentTexture.right;
		_vertex[2].v = mCurrentTexture.top;

		// second triangle - right top
		_vertex[3].x = vertex_right;
		_vertex[3].y = vertex_top;
		_vertex[3].z = vertex_z;
		_vertex[3].colour = mCurrentAlpha;
		_vertex[3].u = mCurrentTexture.right;
		_vertex[3].v = mCurrentTexture.top;

		// second triangle = left bottom
		_vertex[4].x = vertex_left;
		_vertex[4].y = vertex_bottom;
		_vertex[4].z = vertex_z;
		_vertex[4].colour = mCurrentAlpha;
		_vertex[4].u = mCurrentTexture.left;
		_vertex[4].v = mCurrentTexture.bottom;

		// second triangle - right botton
		_vertex[5].x = vertex_right;
		_vertex[5].y = vertex_bottom;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mCurrentAlpha;
		_vertex[5].u = mCurrentTexture.right;
		_vertex[5].v = mCurrentTexture.bottom;

		_count += MAINSKIN_COUNT_VERTEX;
	}

	void MainSkin::_createDrawItem(RenderItem * _item)
	{
		mRenderItem = _item;
		mRenderItem->addDrawItem(this, MAINSKIN_COUNT_VERTEX);
	}

	void MainSkin::_destroyDrawItem()
	{
		mRenderItem->removeDrawItem(this, MAINSKIN_COUNT_VERTEX);
	}

} // namespace MyGUI
