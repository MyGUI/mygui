/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_SubSkin.h"
//#include "MyGUI_Utility.h"
//#include "MyGUI_Gui.h"
//#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_RenderItem.h"

namespace MyGUI
{

	const size_t SUBSKIN_COUNT_VERTEX = 6;

	SubSkin::SubSkin(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		CroppedRectangleInterface(_info.coord, _info.align, _parent),
		mTransparent(false),
		mRenderItem(null),
		mCurrentCoord(_info.coord),
		mCurrentAlpha(0xFFFFFFFF)
	{
	}

	SubSkin::~SubSkin()
	{
	}

	void SubSkin::show()
	{
		if (mShow) return;
		mShow = true;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::hide()
	{
		if (false == mShow) return;
		mShow = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::setAlpha(float _alpha)
	{
		mCurrentAlpha = 0x00FFFFFF | ((uint8)(_alpha*255) << 24);
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::_correctView()
	{
		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::_setAlign(const IntCoord& _coord, bool _update)
	{
		_setAlign(_coord.size(), _update);
	}

	void SubSkin::_setAlign(const IntSize& _size, bool _update)
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
			}
			else {
				// ������� �� ������� ����
				mCoord.left = mCoord.left + (mParent->getWidth() - _size.width);
				need_update = true;
			}

		}
		else if (false == IS_ALIGN_LEFT(mAlign)) {
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
			}
			else {
				mCoord.top = mCoord.top + (mParent->getHeight() - _size.height);
				need_update = true;
			}
		}
		else if (false == IS_ALIGN_TOP(mAlign)) {
			// ������������ �� ��������� ��� ����������
			mCoord.top = (mParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update) {
			mCurrentCoord = mCoord;
			_updateView();
		}

	}

	void SubSkin::_updateView()
	{
		bool margin = _checkMargin();

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// ������� ���� �����
		if (margin) {

			// �������� �� ������ ����� �� �������
			if (_checkOutside()) {

				// ��������
				if (false == mTransparent) mTransparent = true;
				// ���������� ������� ���������
				mIsMargin = margin;

				// �������� ����� �������
				if (null != mRenderItem) mRenderItem->outOfDate();
				return;

			}
		}

		if ((mIsMargin) || (margin)) { // �� �������� ��� ���� ��������

			mCurrentCoord.width = getViewWidth();
			mCurrentCoord.height = getViewHeight();

			if ((mCurrentCoord.width > 0) && (mCurrentCoord.height > 0)) {

				// ������ ������� ��������
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

				mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);
			}
		}

		if ((mIsMargin) && (false == margin)) {
			// �� �� ��������, �� ����, ������ ������� ����������
			mCurrentTexture = mRectTexture;
		}

		// ���������� ������� ���������
		mIsMargin = margin;

		// ���� ���� ��� �����, �� �������
		if (mTransparent) mTransparent = false;

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void SubSkin::_setUVSet(const FloatRect& _rect)
	{
		mRectTexture = _rect;

		// ���� ��������, �� ������������ � ����� �������
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

			mCurrentTexture.set(UV_lft_total, UV_top_total, UV_rgt_total, UV_btm_total);

		}

		// �� �� ��������, ������� ����������
		else {
			mCurrentTexture = mRectTexture;

		}

		if (null != mRenderItem) mRenderItem->outOfDate();

	}

	void SubSkin::_drawItem(Vertex * _vertex, size_t & _count)
	{
		if ((false == mShow) || (mTransparent)) return;
		if ((0 >= mCurrentCoord.width) || (0 >= mCurrentCoord.height)) return;

		float vertex_z = mRenderItem->getMaximumDepth();

		float vertex_left = ((mRenderItem->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mRenderItem->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mRenderItem->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((mRenderItem->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mRenderItem->getVOffset()) * 2) - 1);
		float vertex_bottom = vertex_top - (mRenderItem->getPixScaleY() * (float)mCurrentCoord.height * 2);

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

		_count += SUBSKIN_COUNT_VERTEX;
	}

	void SubSkin::_createDrawItem(RenderItem * _item)
	{
		mRenderItem = _item;
		mRenderItem->addDrawItem(this, SUBSKIN_COUNT_VERTEX);
	}

	void SubSkin::_destroyDrawItem()
	{
		mRenderItem->removeDrawItem(this, SUBSKIN_COUNT_VERTEX);
	}

} // namespace MyGUI
