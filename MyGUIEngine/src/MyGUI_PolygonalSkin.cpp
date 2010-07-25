/*!
	@file
	@author		George Evmenov
	@date		07/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_PolygonalSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_CommonStateInfo.h"

namespace MyGUI
{

	PolygonalSkin::PolygonalSkin() :
		mGeometryOutdated(false),
		mLineWidth(1.0f),
		mLineLength(0.0f),
		mVertexCount(0),
		mEmptyView(false),
		mNode(nullptr),
		mRenderItem(nullptr)
	{
	}

	PolygonalSkin::~PolygonalSkin()
	{
	}

	inline float len(float x, float y) { return sqrt(x*x + y*y); }

	void PolygonalSkin::setPoints(const std::vector<FloatPoint>& _points)
	{
		mLinePoints = _points;
		
		if (mLinePoints.size() < 2)
		{
			mVertexCount = 0;
			mResultVerticiesPos.clear();
			mResultVerticiesUV.clear();
			return;
		}

		// нужно больше вершин
		size_t count = (mLinePoints.size() - 1) * VertexQuad::VertexCount;
		if (count > mVertexCount)
		{
			mVertexCount = count;
			if (nullptr != mRenderItem) mRenderItem->reallockDrawItem(this, mVertexCount);
		}

		mLineLength = 0.0f;
		FloatPoint point = mLinePoints[0];
		for (std::vector<FloatPoint>::iterator iter = mLinePoints.begin() + 1; iter != mLinePoints.end(); ++iter)
		{
			mLineLength += len(iter->left - point.left, iter->top - point.top);
			point = *iter;
		}

		mGeometryOutdated = true;
	}

	void PolygonalSkin::setWidth(float _width)
	{
		mLineWidth = _width;
		mGeometryOutdated = true;
	}

	void PolygonalSkin::setVisible(bool _visible)
	{
		if (mVisible == _visible)
			return;

		mVisible = _visible;
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::setAlpha(float _alpha)
	{
		uint32 alpha = ((uint8)(_alpha*255) << 24);
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

	void PolygonalSkin::_setAlign(const IntSize& _oldsize/*, bool _update*/)
	{
		// необходимо разобраться
		bool need_update = true;//_update;

		// первоначальное выравнивание
		if (mAlign.isHStretch())
		{
			// растягиваем
			mCoord.width = mCoord.width + (mCroppedParent->getWidth() - _oldsize.width);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isRight())
		{
			// двигаем по правому краю
			mCoord.left = mCoord.left + (mCroppedParent->getWidth() - _oldsize.width);
			need_update = true;
		}
		else if (mAlign.isHCenter())
		{
			// выравнивание по горизонтали без растяжения
			mCoord.left = (mCroppedParent->getWidth() - mCoord.width) / 2;
			need_update = true;
		}

		if (mAlign.isVStretch())
		{
			// растягиваем
			mCoord.height = mCoord.height + (mCroppedParent->getHeight() - _oldsize.height);
			need_update = true;
			mIsMargin = true; // при изменении размеров все пересчитывать
		}
		else if (mAlign.isBottom())
		{
			// двигаем по нижнему краю
			mCoord.top = mCoord.top + (mCroppedParent->getHeight() - _oldsize.height);
			need_update = true;
		}
		else if (mAlign.isVCenter())
		{
			// выравнивание по вертикали без растяжения
			mCoord.top = (mCroppedParent->getHeight() - mCoord.height) / 2;
			need_update = true;
		}

		if (need_update)
		{
			mCurrentCoord = mCoord;
			_updateView();
		}
	}

	void PolygonalSkin::_updateView()
	{
		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::createDrawItem(ITexture* _texture, ILayerNode * _node)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		mNode = _node;
		mRenderItem = mNode->addToRenderItem(_texture, this);
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
		if ((!mVisible) || mEmptyView)
			return;

		Vertex* verticies = mRenderItem->getCurrentVertextBuffer();

		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();

		float vertex_z = info.maximumDepth;

		if (mGeometryOutdated)
		{
			_rebuildGeometry();
			mGeometryOutdated = false;
		}

		if (mVertexCount != 0)
		{
			// set verticies as tristrip
			size_t triangles = mResultVerticiesPos.size() - 2;
			bool odd = false;
			for (size_t i = 0; i < triangles; ++i)
			{
				int j = i + (odd ? 2 : 1);
				int k = i + (odd ? 1 : 2);
				verticies[3*i    ].set(mResultVerticiesPos[i].left, mResultVerticiesPos[i].top, vertex_z, mResultVerticiesUV[i].left, mResultVerticiesUV[i].top, mCurrentColour);
				verticies[3*i + 1].set(mResultVerticiesPos[j].left, mResultVerticiesPos[j].top, vertex_z, mResultVerticiesUV[j].left, mResultVerticiesUV[j].top, mCurrentColour);
				verticies[3*i + 2].set(mResultVerticiesPos[k].left, mResultVerticiesPos[k].top, vertex_z, mResultVerticiesUV[k].left, mResultVerticiesUV[k].top, mCurrentColour);
				odd = !odd;
			}
		}

		mRenderItem->setLastVertexCount(mVertexCount);
	}

	void PolygonalSkin::_setColour(const Colour& _value)
	{
		uint32 colour = texture_utility::toColourARGB(_value);
		texture_utility::convertColour(colour, mVertexFormat);
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
		
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void PolygonalSkin::_rebuildGeometry()
	{
#ifndef M_PI
		const float M_PI = 3.141593f;
#endif
		if (mLinePoints.size() < 2) return;

		// using mCurrentCoord as rectangle where we draw polygons

		// base texture coordinates
		FloatPoint baseVerticiesUV[4] =
		{
			FloatPoint(mCurrentTexture.left, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.bottom),
			FloatPoint(mCurrentTexture.left, mCurrentTexture.bottom)
		};

		// UV vectors
		FloatPoint vectorU = baseVerticiesUV[1] - baseVerticiesUV[0];
		FloatPoint vectorV = baseVerticiesUV[3] - baseVerticiesUV[0];

		FloatPoint vertex1;
		FloatPoint vertex2;
		mResultVerticiesPos.clear();
		mResultVerticiesUV.clear();
		// add first two verticies
		FloatPoint normal = _getPerpendicular(mLinePoints[0], mLinePoints[1]);
		mResultVerticiesPos.push_back(mLinePoints[0] + normal);
		mResultVerticiesPos.push_back(mLinePoints[0] - normal);
		mResultVerticiesUV.push_back(baseVerticiesUV[0]);
		mResultVerticiesUV.push_back(baseVerticiesUV[3]);

		// add other verticies
		float currentLength = 0.0f;
		size_t i;
		for (i = 1; i < mLinePoints.size() - 1; ++i)
		{
			currentLength += len(mLinePoints[i-1].left - mLinePoints[i].left,  mLinePoints[i-1].top - mLinePoints[i].top);

			// getting normal between previous and next point
			//normal = _getPerpendicular(mLinePoints[i-1], mLinePoints[i+1]);
			normal = _getMiddleLine(mLinePoints[i-1], mLinePoints[i+1], mLinePoints[i]);

			// check orientation
			FloatPoint lineDir = mLinePoints[i+1] - mLinePoints[i-1];
			if (lineDir.left * normal.top - lineDir.top * normal.left < 0)
			{
				normal.left = -normal.left;
				normal.top = -normal.top;
			}

			mResultVerticiesPos.push_back(mLinePoints[i] + normal);
			mResultVerticiesPos.push_back(mLinePoints[i] - normal);
			FloatPoint UVoffset(currentLength / mLineLength * vectorU.left, currentLength / mLineLength * vectorU.top);
			mResultVerticiesUV.push_back(baseVerticiesUV[0] + UVoffset);
			mResultVerticiesUV.push_back(baseVerticiesUV[3] + UVoffset);
		}

		// add last two verticies
		normal = _getPerpendicular(mLinePoints[i-1], mLinePoints[i]);
		mResultVerticiesPos.push_back(mLinePoints[i] + normal);
		mResultVerticiesPos.push_back(mLinePoints[i] - normal);
		mResultVerticiesUV.push_back(baseVerticiesUV[1]);
		mResultVerticiesUV.push_back(baseVerticiesUV[2]);

		// now calculate widget base offset and then resulting position in screen coordinates
		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();
		float vertex_left_base = ((info.pixScaleX * (float)(mCroppedParent->getAbsoluteLeft()) + info.hOffset) * 2) - 1;
		float vertex_top_base = -(((info.pixScaleY * (float)(mCroppedParent->getAbsoluteTop()) + info.vOffset) * 2) - 1);

		for (size_t i = 0; i < mResultVerticiesPos.size(); ++i)
		{
			mResultVerticiesPos[i].left = vertex_left_base + mResultVerticiesPos[i].left * info.pixScaleX * 2;
			mResultVerticiesPos[i].top = vertex_top_base + mResultVerticiesPos[i].top * info.pixScaleY * -2;
		}
	}

	FloatPoint PolygonalSkin::_getPerpendicular(const FloatPoint& _point1, const FloatPoint& _point2)
	{
		// dy, -dx
		FloatPoint result(_point1.top - _point2.top, -(_point1.left - _point2.left));
		// normalise
		float length = len(result.top, result.left);
		result.left /= length;
		result.top /= length;
		result.left *= mLineWidth/2;
		result.top *= mLineWidth/2;
		return result;
	}

	FloatPoint PolygonalSkin::_getMiddleLine(const FloatPoint& _point1, const FloatPoint& _point2, const FloatPoint& _point3)
	{
		/*
		// median
		FloatPoint result = FloatPoint() - (_point1 + _point2);
		result.left /= 2;
		result.top /= 2;
		result += _point3;
		// normalise
		float length = len(result.top, result.left);
		result.left /= length;
		result.top /= length;
		result.left *= mLineWidth/2;
		result.top *= mLineWidth/2;
		return result;
		*/

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
		result.left /= length;
		result.top /= length;

		float cos = result.left*line1.left + result.top*line1.top;
		float angle = acos(cos);
		float width = mLineWidth/2 / sin(angle);

		result.left *= width;
		result.top *= width;
		return result;
	}

} // namespace MyGUI
