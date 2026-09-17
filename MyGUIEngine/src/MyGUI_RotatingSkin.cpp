/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_GeometryUtility.h"

namespace MyGUI
{

	RotatingSkin::RotatingSkin()
	{
		mVertexFormat = RenderManager::getInstance().getVertexFormat();
	}

	void RotatingSkin::setAngle(float _angle)
	{
		mAngle = _angle;
		_correctView();
	}

	void RotatingSkin::setCenter(const IntPoint& _center)
	{
		mCenterPos = _center;
		_correctView();
	}

	IntPoint RotatingSkin::getCenter(bool _local) const
	{
		return mCenterPos + (_local ? IntPoint() : mCroppedParent->getAbsolutePosition());
	}

	void RotatingSkin::setVisible(bool _visible)
	{
		if (mVisible == _visible)
			return;

		mVisible = _visible;
		_correctView();
	}

	void RotatingSkin::setAlpha(float _alpha)
	{
		uint32 alpha = ((uint8)(_alpha * 255) << 24);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | (alpha & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::_correctView()
	{
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::_setAlign(const IntSize& _oldsize)
	{
		auto [coord, resize, move] = CoordConverter::applyAlign(mAlign, mCoord, mCroppedParent->getSize(), _oldsize);
		mCoord = coord;
		if (resize)
			mIsMargin = true;
		mCurrentCoord = mCoord;
		_updateView();
	}

	void RotatingSkin::_updateView()
	{
		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		_correctView();
	}

	void RotatingSkin::createDrawItem(ITexture* _texture, ILayerNode* _node)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		mNode = _node;
		mRenderItem = mNode->addToRenderItem(_texture, true, false);
		mRenderItem->addDrawItem(this, (GEOMETRY_VERTICIES_TOTAL_COUNT - 2) * 3);
	}

	void RotatingSkin::destroyDrawItem()
	{
		MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");

		mNode = nullptr;
		mRenderItem->removeDrawItem(this);
		mRenderItem = nullptr;
	}

	void RotatingSkin::doRender()
	{
		if (!mVisible || mEmptyView)
			return;

		Vertex* verticies = mRenderItem->getCurrentVertexBuffer();

		float vertex_z = mNode->getNodeDepth();

		if (mGeometryOutdated || mRenderItem->getCurrentUpdate())
		{
			_rebuildGeometry();
			mGeometryOutdated = false;
		}

		for (size_t i = 1; i + 1 < mVertexCount; ++i)
		{
			verticies[3 * i - 3].set(
				mResultVerticiesPos[0].left,
				mResultVerticiesPos[0].top,
				vertex_z,
				mResultVerticiesUV[0].left,
				mResultVerticiesUV[0].top,
				mCurrentColour);
			verticies[3 * i - 2].set(
				mResultVerticiesPos[i].left,
				mResultVerticiesPos[i].top,
				vertex_z,
				mResultVerticiesUV[i].left,
				mResultVerticiesUV[i].top,
				mCurrentColour);
			verticies[3 * i - 1].set(
				mResultVerticiesPos[i + 1].left,
				mResultVerticiesPos[i + 1].top,
				vertex_z,
				mResultVerticiesUV[i + 1].left,
				mResultVerticiesUV[i + 1].top,
				mCurrentColour);
		}

		mRenderItem->setLastVertexCount(mVertexCount < 3 ? 0 : (mVertexCount - 2) * 3);
	}

	void RotatingSkin::_setColour(const Colour& _value)
	{
		uint32 colour = texture_utility::toNativeColour(_value, mVertexFormat);
		mCurrentColour = (colour & 0x00FFFFFF) | (mCurrentColour & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::setStateData(IStateInfo* _data)
	{
		RotatingSkinStateInfo* data = _data->castType<RotatingSkinStateInfo>();

		setAngle(data->getAngle());
		setCenter(data->getCenter());

		_setUVSet(data->getRect());
	}

	void RotatingSkin::_setUVSet(const FloatRect& _rect)
	{
		mCurrentTexture = _rect;

		_correctView();
	}

	void RotatingSkin::_rebuildGeometry()
	{
		// Rotate the four corners around the widget-local center.
		const float left = (float)mCurrentCoord.left;
		const float top = (float)mCurrentCoord.top;
		const float right = (float)mCurrentCoord.right();
		const float bottom = (float)mCurrentCoord.bottom();
		FloatPoint corners[RECT_VERTICIES_COUNT] = {{left, top}, {right, top}, {right, bottom}, {left, bottom}};
		const float cos = std::cos(mAngle);
		const float sin = std::sin(mAngle);
		for (auto& point : corners)
		{
			const float x = point.left - mCenterPos.left;
			const float y = point.top - mCenterPos.top;
			point = {mCenterPos.left + x * cos - y * sin, mCenterPos.top + x * sin + y * cos};
		}

		mVertexCount = RECT_VERTICIES_COUNT;
		std::copy(corners, corners + RECT_VERTICIES_COUNT, mResultVerticiesPos);
		if (ICroppedRectangle* parent = mCroppedParent->getCroppedParent())
		{
			const VectorFloatPoint points = geometry_utility::cropPolygon(
				corners,
				RECT_VERTICIES_COUNT,
				IntCoord(
					parent->_getMarginLeft() - mCroppedParent->getLeft(),
					parent->_getMarginTop() - mCroppedParent->getTop(),
					parent->_getViewWidth(),
					parent->_getViewHeight()));
			mVertexCount = points.size();
			std::copy(points.begin(), points.end(), mResultVerticiesPos);
		}

		const FloatPoint baseUV(mCurrentTexture.left, mCurrentTexture.top);
		const FloatPoint u(mCurrentTexture.right - mCurrentTexture.left, 0);
		const FloatPoint v(0, mCurrentTexture.bottom - mCurrentTexture.top);
		for (size_t i = 0; i < mVertexCount; ++i)
		{
			const FloatPoint position =
				geometry_utility::getPositionInsideRect(mResultVerticiesPos[i], corners[0], corners[1], corners[3]);
			mResultVerticiesUV[i] = geometry_utility::getUVFromPositionInsideRect(position, v, u, baseUV);
		}

		geometry_utility::toRenderTarget(
			mResultVerticiesPos,
			mVertexCount,
			mCroppedParent->getAbsolutePosition(),
			mRenderItem->getRenderTarget()->getInfo());
	}

	float RotatingSkin::getAngle() const
	{
		return mAngle;
	}

} // namespace MyGUI
