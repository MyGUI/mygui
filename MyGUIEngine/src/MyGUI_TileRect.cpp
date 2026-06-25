/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_TileRect.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	const size_t TILERECT_COUNT_VERTEX = 16 * VertexQuad::VertexCount;

	TileRect::TileRect() :
		mCountVertex(TILERECT_COUNT_VERTEX)
	{
		mVertexFormat = RenderManager::getInstance().getVertexFormat();
	}

	void TileRect::setVisible(bool _visible)
	{
		if (mVisible == _visible)
			return;
		mVisible = _visible;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void TileRect::setAlpha(float _alpha)
	{
		uint32 alpha = ((uint8)(_alpha * 255) << 24);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | (alpha & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void TileRect::_correctView()
	{
		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void TileRect::_setAlign(const IntSize& _oldsize)
	{
		auto [coord, resize, move] = CoordConverter::applyAlign(mAlign, mCoord, mCroppedParent->getSize(), _oldsize);
		mCoord = coord;
		if (resize)
			mIsMargin = true;
		mCurrentCoord = mCoord;
		if (!mTileH)
			mTileSize.width = mCoord.width;
		if (!mTileV)
			mTileSize.height = mCoord.height;
		_updateView();
	}

	void TileRect::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;
		mCurrentCoord.width = _getViewWidth();
		mCurrentCoord.height = _getViewHeight();

		// calculate required number of tiles
		if (!mEmptyView)
		{
			size_t count = 0;
			if (!mTileSize.empty())
			{
				size_t count_x = mCoord.width / mTileSize.width;
				if ((mCoord.width % mTileSize.width) > 0)
					count_x++;

				size_t count_y = mCoord.height / mTileSize.height;
				if ((mCoord.height % mTileSize.height) > 0)
					count_y++;

				count = count_y * count_x * VertexQuad::VertexCount;
			}

			// need more vertices
			if (count > mCountVertex)
			{
				mCountVertex = count + TILERECT_COUNT_VERTEX;
				if (nullptr != mRenderItem)
					mRenderItem->reallockDrawItem(this, mCountVertex);
			}
		}

		// viewport became invalid
		if (margin)
		{
			if (_checkOutside())
			{
				// remember current state
				mIsMargin = margin;

				// update before exit
				if (nullptr != mNode)
					mNode->outOfDate(mRenderItem);
				return;
			}
		}

		// remember current state
		mIsMargin = margin;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void TileRect::_setUVSet(const FloatRect& _rect)
	{
		mCurrentTexture = _rect;
		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void TileRect::doRender()
	{
		if (!mVisible || mEmptyView || mTileSize.empty())
			return;

		VertexQuad* quad = reinterpret_cast<VertexQuad*>(mRenderItem->getCurrentVertexBuffer());

		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();

		// size of one tile
		mRealTileWidth = info.pixScaleX * (float)(mTileSize.width) * 2;
		mRealTileHeight = info.pixScaleY * (float)(mTileSize.height) * 2;

		mTextureHeightOne = (mCurrentTexture.bottom - mCurrentTexture.top) / mRealTileHeight;
		mTextureWidthOne = (mCurrentTexture.right - mCurrentTexture.left) / mRealTileWidth;

		float vertex_z = mNode->getNodeDepth();

		// absolute window size
		float window_left =
			((info.pixScaleX * (float)(mCoord.left + mCroppedParent->getAbsoluteLeft() - info.leftOffset) +
			  info.hOffset) *
			 2) -
			1;
		float window_top = -(
			((info.pixScaleY * (float)(mCoord.top + mCroppedParent->getAbsoluteTop() - info.topOffset) + info.vOffset) *
			 2) -
			1);

		// viewport size
		float real_left =
			((info.pixScaleX * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft() - info.leftOffset) +
			  info.hOffset) *
			 2) -
			1;
		float real_right = real_left + (info.pixScaleX * (float)mCurrentCoord.width * 2);
		float real_top =
			-(((info.pixScaleY * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() - info.topOffset) +
				info.vOffset) *
			   2) -
			  1);
		float real_bottom = real_top - (info.pixScaleY * (float)mCurrentCoord.height * 2);

		size_t count = 0;

		float bottom = window_top;

		for (int y = 0; y < mCoord.height; y += mTileSize.height)
		{
			float top = bottom;
			bottom -= mRealTileHeight;
			float right = window_left;

			float vertex_top = top;
			float vertex_bottom = bottom;
			bool texture_crop_height = false;

			if (vertex_top > real_top)
			{
				// check for complete out-of-bounds
				if (vertex_bottom > real_top)
				{
					continue;
				}
				vertex_top = real_top;
				texture_crop_height = true;
			}
			if (vertex_bottom < real_bottom)
			{
				// went completely out at bottom
				if (vertex_top < real_bottom)
				{
					continue;
				}
				vertex_bottom = real_bottom;
				texture_crop_height = true;
			}

			for (int x = 0; x < mCoord.width; x += mTileSize.width)
			{
				float left = right;
				right += mRealTileWidth;

				float vertex_left = left;
				float vertex_right = right;
				bool texture_crop_width = false;


				if (vertex_left < real_left)
				{
					// check for complete out-of-bounds
					if (vertex_right < real_left)
					{
						continue;
					}
					vertex_left = real_left;
					texture_crop_width = true;
				}

				if (vertex_right > real_right)
				{
					// don't need the full row
					if (vertex_left > real_right)
					{
						continue;
					}
					vertex_right = real_right;
					texture_crop_width = true;
				}

				// current texture coordinates
				float texture_left = mCurrentTexture.left;
				float texture_right = mCurrentTexture.right;
				float texture_top = mCurrentTexture.top;
				float texture_bottom = mCurrentTexture.bottom;

				// texture offset vertically
				if (texture_crop_height)
				{
					// add offset size in texture coordinates
					texture_top += (top - vertex_top) * mTextureHeightOne;
					// subtract offset size in texture coordinates
					texture_bottom -= (vertex_bottom - bottom) * mTextureHeightOne;
				}

				// texture offset horizontally
				if (texture_crop_width)
				{
					// add offset size in texture coordinates
					texture_left += (vertex_left - left) * mTextureWidthOne;
					// subtract offset size in texture coordinates
					texture_right -= (right - vertex_right) * mTextureWidthOne;
				}

				quad[count].set(
					vertex_left,
					vertex_top,
					vertex_right,
					vertex_bottom,
					vertex_z,
					texture_left,
					texture_top,
					texture_right,
					texture_bottom,
					mCurrentColour);

				count++;
			}
		}

		mRenderItem->setLastVertexCount(VertexQuad::VertexCount * count);
	}

	void TileRect::createDrawItem(ITexture* _texture, ILayerNode* _node)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		mNode = _node;
		mRenderItem = mNode->addToRenderItem(_texture, true, false);
		mRenderItem->addDrawItem(this, mCountVertex);
	}

	void TileRect::destroyDrawItem()
	{
		MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");

		mNode = nullptr;
		mRenderItem->removeDrawItem(this);
		mRenderItem = nullptr;
	}

	void TileRect::setStateData(IStateInfo* _data)
	{
		TileRectStateInfo* data = _data->castType<TileRectStateInfo>();

		mTileSize = data->getTileSize();
		mTileH = data->getTileH();
		mTileV = data->getTileV();
		_setUVSet(data->getRect());
	}

	void TileRect::_setColour(const Colour& _value)
	{
		uint32 colour = texture_utility::toNativeColour(_value, mVertexFormat);
		mCurrentColour = (colour & 0x00FFFFFF) | (mCurrentColour & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

} // namespace MyGUI
