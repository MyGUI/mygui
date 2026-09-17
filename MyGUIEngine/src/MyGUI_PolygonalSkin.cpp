/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_PolygonalSkin.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_GeometryUtility.h"

namespace MyGUI
{

	PolygonalSkin::PolygonalSkin()
	{
		mVertexFormat = RenderManager::getInstance().getVertexFormat();
	}

	inline float len(float x, float y)
	{
		return std::sqrt(x * x + y * y);
	}

	void PolygonalSkin::setPoints(const std::vector<FloatPoint>& _points)
	{
		mLinePoints.clear();
		mLinePoints.reserve(_points.size());
		mLineLength = 0;
		for (const auto& point : _points)
		{
			if (!mLinePoints.empty())
			{
				const auto& previous = mLinePoints.back();
				if (previous == point)
					continue;
				mLineLength += len(point.left - previous.left, point.top - previous.top);
			}
			mLinePoints.push_back(point);
		}

		const size_t segments = mLinePoints.empty() ? 0 : mLinePoints.size() - 1;
		// Each segment emits two triangles, plus at most two triangles for its join.
		size_t count = segments * 12;
#ifndef MYGUI_NO_POLYGONAL_SKIN_CROPPING
		// Clipping a triangle against four sides adds at most four vertices:
		// seven vertices triangulate into at most five triangles.
		count *= 5;
#endif
		if (count > mVertexCount)
		{
			mVertexCount = count;
			if (nullptr != mRenderItem)
				mRenderItem->reallockDrawItem(this, mVertexCount);
		}
		_updateView();
	}

	void PolygonalSkin::setWidth(float _width)
	{
		mLineWidth = _width;
		_updateView();
	}

	void PolygonalSkin::setStroke(size_t _value)
	{
		mLineStroke = _value;
		_updateView();
	}

	void PolygonalSkin::setVisible(bool _visible)
	{
		if (mVisible == _visible)
			return;

		mVisible = _visible;
		_correctView();
	}

	void PolygonalSkin::setAlpha(float _alpha)
	{
		uint32 alpha = ((uint8)(_alpha * 255) << 24);
		mCurrentColour = (mCurrentColour & 0x00FFFFFF) | (alpha & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::_correctView()
	{
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::_setAlign(const IntSize& _oldsize)
	{
		auto [coord, resize, move] = CoordConverter::applyAlign(mAlign, mCoord, mCroppedParent->getSize(), _oldsize);
		mCoord = coord;
		if (resize)
			mIsMargin = true;
		mCurrentCoord = mCoord;
		_updateView();
	}

	void PolygonalSkin::_updateView()
	{
		bool margin = _checkMargin();

		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mGeometryOutdated = true;

		mCurrentCoord.left = mCoord.left + mMargin.left;
		mCurrentCoord.top = mCoord.top + mMargin.top;

		// viewport changed
		if (margin)
		{
			if (_checkOutside())
			{
				mIsMargin = margin;

				if (nullptr != mNode)
					mNode->outOfDate(mRenderItem);
				return;
			}
		}

		// we are clipped or were clipped
		if (mIsMargin || margin)
		{
			mCurrentCoord.width = _getViewWidth();
			mCurrentCoord.height = _getViewHeight();
		}

		mIsMargin = margin;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::createDrawItem(ITexture* _texture, ILayerNode* _node)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		mNode = _node;
		mRenderItem = mNode->addToRenderItem(_texture, true, false);
		mRenderItem->addDrawItem(this, mVertexCount);
	}

	void PolygonalSkin::destroyDrawItem()
	{
		MYGUI_ASSERT(mRenderItem, "mRenderItem must be not nullptr");

		mNode = nullptr;
		mRenderItem->removeDrawItem(this);
		mRenderItem = nullptr;
	}

	void PolygonalSkin::doRender()
	{
		if (!mVisible || mEmptyView)
			return;

		Vertex* verticies = mRenderItem->getCurrentVertexBuffer();

		float vertex_z = mNode->getNodeDepth();

		if (mGeometryOutdated || mRenderItem->getCurrentUpdate())
		{
			_rebuildGeometry();
		}

		size_t size = mResultVerticiesPos.size();

		for (size_t i = 0; i < size; ++i)
		{
			verticies[i].set(
				mResultVerticiesPos[i].left,
				mResultVerticiesPos[i].top,
				vertex_z,
				mResultVerticiesUV[i].left,
				mResultVerticiesUV[i].top,
				mCurrentColour);
		}

		mRenderItem->setLastVertexCount(size);
	}

	void PolygonalSkin::_setColour(const Colour& _value)
	{
		uint32 colour = texture_utility::toNativeColour(_value, mVertexFormat);
		mCurrentColour = (colour & 0x00FFFFFF) | (mCurrentColour & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::setStateData(IStateInfo* _data)
	{
		_setUVSet(_data->castType<SubSkinStateInfo>()->getRect());
	}

	void PolygonalSkin::_setUVSet(const FloatRect& _rect)
	{
		mCurrentTexture = _rect;

		_correctView();
	}

	void PolygonalSkin::_rebuildGeometry()
	{
		if (!mRenderItem || !mRenderItem->getRenderTarget())
			return;

		mGeometryOutdated = false;
		mResultVerticiesPos.clear();
		mResultVerticiesUV.clear();
		if (mLinePoints.size() < 2)
			return;

		// using mCurrentCoord as rectangle where we draw polygons

		// base texture coordinates
		FloatPoint baseVerticiesUV[4] = {
			FloatPoint(mCurrentTexture.left, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.bottom),
			FloatPoint(mCurrentTexture.left, mCurrentTexture.bottom)};

		// UV vectors
		FloatPoint vectorU = baseVerticiesUV[1] - baseVerticiesUV[0];
		//FloatPoint vectorV = baseVerticiesUV[3] - baseVerticiesUV[0];

		// add first two verticies
		FloatPoint normal = _getPerpendicular(mLinePoints[0], mLinePoints[1]);

		FloatPoint points[2] = {mLinePoints[0] + normal, mLinePoints[0] - normal};
		FloatPoint pointsUV[2] = {baseVerticiesUV[0], baseVerticiesUV[3]};

		bool draw = true;
		size_t stroke = 0;

		// add other verticies
		float currentLength = 0.0f;
		for (size_t i = 1; i < mLinePoints.size(); ++i)
		{
			if (mLineStroke != 0)
			{
				stroke++;
				if (stroke == mLineStroke)
				{
					stroke = 0;
					draw = !draw;
				}
			}

			currentLength +=
				len(mLinePoints[i - 1].left - mLinePoints[i].left, mLinePoints[i - 1].top - mLinePoints[i].top);

			// getting normal between previous and next point
			if (i != mLinePoints.size() - 1)
				normal = _getMiddleLine(mLinePoints[i - 1], mLinePoints[i + 1], mLinePoints[i]);
			else
				normal = _getPerpendicular(mLinePoints[i - 1], mLinePoints[i]);

			bool edge = false;
			bool sharp = false;
			if (normal == FloatPoint())
			{
				edge = true;
				normal = _getPerpendicular(mLinePoints[i - 1], mLinePoints[i]);
			}
			else if (len(normal.left, normal.top) > mLineWidth * 1.5f)
			{
				sharp = true;
				normal = _getPerpendicular(mLinePoints[i - 1], mLinePoints[i]);
			}

			// check orientation
			FloatPoint lineDir = mLinePoints[i] - mLinePoints[i - 1];
			if (lineDir.left * normal.top - lineDir.top * normal.left < 0)
			{
				normal.left = -normal.left;
				normal.top = -normal.top;
			}

			FloatPoint UVoffset(currentLength / mLineLength * vectorU.left, currentLength / mLineLength * vectorU.top);

			if (draw)
			{
				mResultVerticiesPos.push_back(points[0]);
				mResultVerticiesPos.push_back(points[1]);
				mResultVerticiesPos.push_back(mLinePoints[i] + normal);
				mResultVerticiesUV.push_back(pointsUV[0]);
				mResultVerticiesUV.push_back(pointsUV[1]);
				mResultVerticiesUV.push_back(baseVerticiesUV[0] + UVoffset);

				mResultVerticiesPos.push_back(points[1]);
				mResultVerticiesPos.push_back(mLinePoints[i] - normal);
				mResultVerticiesPos.push_back(mLinePoints[i] + normal);
				mResultVerticiesUV.push_back(pointsUV[1]);
				mResultVerticiesUV.push_back(baseVerticiesUV[3] + UVoffset);
				mResultVerticiesUV.push_back(baseVerticiesUV[0] + UVoffset);
			}

			points[edge ? 1 : 0] = mLinePoints[i] + normal;
			points[edge ? 0 : 1] = mLinePoints[i] - normal;
			pointsUV[0] = baseVerticiesUV[0] + UVoffset;
			pointsUV[1] = baseVerticiesUV[3] + UVoffset;

			if (sharp)
			{
				normal = _getMiddleLine(mLinePoints[i - 1], mLinePoints[i + 1], mLinePoints[i]);

				float length = len(normal.left, normal.top);
				float sharpness = length / mLineWidth;
				normal.left *= 2 * mLineWidth / length / (sharpness - 0.5f);
				normal.top *= 2 * mLineWidth / length / (sharpness - 0.5f);

				// check orientation
				lineDir = mLinePoints[i] - mLinePoints[i - 1];
				if (lineDir.left * normal.top - lineDir.top * normal.left < 0)
				{
					normal.left = -normal.left;
					normal.top = -normal.top;
				}
				FloatPoint lineDir1 = mLinePoints[i] - mLinePoints[i - 1];
				FloatPoint lineDir2 = mLinePoints[i + 1] - mLinePoints[i];
				if (lineDir1.left * lineDir2.top - lineDir1.top * lineDir2.left > 0)
				{
					normal.left = -normal.left;
					normal.top = -normal.top;
				}

				// check orientation
				FloatPoint normal2 = _getPerpendicular(mLinePoints[i], mLinePoints[i + 1]);
				lineDir = mLinePoints[i - 1] - mLinePoints[i];
				if (lineDir.left * normal2.top - lineDir.top * normal2.left < 0)
				{
					normal2.left = -normal2.left;
					normal2.top = -normal2.top;
				}

				FloatPoint UVcenter(
					(baseVerticiesUV[0].left + baseVerticiesUV[3].left) / 2,
					(baseVerticiesUV[0].top + baseVerticiesUV[3].top) / 2);

				if (draw)
				{
					mResultVerticiesPos.push_back(points[0]);
					mResultVerticiesPos.push_back(mLinePoints[i] + normal);
					mResultVerticiesPos.push_back(mLinePoints[i]);
					mResultVerticiesUV.push_back(pointsUV[0]);
					mResultVerticiesUV.push_back(baseVerticiesUV[0] + UVoffset);
					mResultVerticiesUV.push_back(UVcenter + UVoffset);

					mResultVerticiesPos.push_back(mLinePoints[i] + normal);
					mResultVerticiesPos.push_back(mLinePoints[i] + normal2);
					mResultVerticiesPos.push_back(mLinePoints[i]);
					mResultVerticiesUV.push_back(baseVerticiesUV[0] + UVoffset);
					mResultVerticiesUV.push_back(baseVerticiesUV[0] + UVoffset);
					mResultVerticiesUV.push_back(UVcenter + UVoffset);
				}

				points[0] = mLinePoints[i] + normal2;
				points[1] = mLinePoints[i] - normal2;
				pointsUV[0] = baseVerticiesUV[0] + UVoffset;
				pointsUV[1] = baseVerticiesUV[3] + UVoffset;
			}
		}


#ifndef MYGUI_NO_POLYGONAL_SKIN_CROPPING
		// crop triangles
		IntCoord cropRectangle(mCurrentCoord.left, mCurrentCoord.top, mCurrentCoord.width, mCurrentCoord.height);

		VectorFloatPoint newResultVerticiesPos;
		VectorFloatPoint newResultVerticiesUV;
		newResultVerticiesPos.reserve(mResultVerticiesPos.size());
		newResultVerticiesUV.reserve(mResultVerticiesPos.size());
		for (size_t i = 0; i < mResultVerticiesPos.size(); i += 3)
		{
			VectorFloatPoint croppedTriangle = geometry_utility::cropPolygon(&mResultVerticiesPos[i], 3, cropRectangle);
			const FloatPoint v0 = mResultVerticiesUV[i + 2] - mResultVerticiesUV[i];
			const FloatPoint v1 = mResultVerticiesUV[i + 1] - mResultVerticiesUV[i];
			for (size_t j = 1; j + 1 < croppedTriangle.size(); ++j)
			{
				for (size_t index : {size_t(0), j, j + 1})
				{
					const FloatPoint& vertex = croppedTriangle[index];
					newResultVerticiesPos.push_back(vertex);
					const FloatPoint position = geometry_utility::getPositionInsideRect(
						vertex,
						mResultVerticiesPos[i],
						mResultVerticiesPos[i + 1],
						mResultVerticiesPos[i + 2]);
					newResultVerticiesUV.push_back(
						geometry_utility::getUVFromPositionInsideRect(position, v0, v1, mResultVerticiesUV[i]));
				}
			}
		}
		std::swap(mResultVerticiesPos, newResultVerticiesPos);
		std::swap(mResultVerticiesUV, newResultVerticiesUV);
#endif


		geometry_utility::toRenderTarget(
			mResultVerticiesPos.data(),
			mResultVerticiesPos.size(),
			mCroppedParent->getAbsolutePosition(),
			mRenderItem->getRenderTarget()->getInfo());
	}

	FloatPoint PolygonalSkin::_getPerpendicular(const FloatPoint& _point1, const FloatPoint& _point2) const
	{
		// dy, -dx
		FloatPoint result(_point1.top - _point2.top, -(_point1.left - _point2.left));
		// normalise
		float length = len(result.top, result.left);
		result.left /= length;
		result.top /= length;
		result.left *= mLineWidth / 2;
		result.top *= mLineWidth / 2;
		return result;
	}

	FloatPoint PolygonalSkin::_getMiddleLine(
		const FloatPoint& _point1,
		const FloatPoint& _point2,
		const FloatPoint& _point3) const
	{
		// bisectrix
		FloatPoint line1 = _point3 - _point1;
		FloatPoint line2 = _point3 - _point2;
		float length = len(line1.top, line1.left);
		line1.left /= length;
		line1.top /= length;
		length = len(line2.top, line2.left);
		line2.left /= length;
		line2.top /= length;
		FloatPoint result = line1 + line2;
		// normalise
		length = len(result.top, result.left);
		if (length < 1e-6f)
		{
			return _getPerpendicular(_point1, _point2);
		}
		result.left /= length;
		result.top /= length;

		float cos = result.left * line1.left + result.top * line1.top;
		float angle = std::acos(std::clamp(cos, -1.0f, 1.0f));

		// too sharp angle
		if (std::fabs(angle) < 1e-6f)
			return {};

		float width = mLineWidth / 2 / std::sin(angle);
		result.left *= width;
		result.top *= width;

		MYGUI_SUPPRESS_CLANG("-Wnrvo")
		return result;
		MYGUI_UNSUPPRESS_CLANG()
	}

} // namespace MyGUI
