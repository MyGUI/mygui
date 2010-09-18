/*!
	@file
	@author		George Evmenov
	@date		05/2009
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
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	RotatingSkin::RotatingSkin() :
		mGeometryOutdated(false),
		mAngle(0.0f),
		mEmptyView(false),
		mCurrentColour(0xFFFFFFFF),
		mNode(nullptr),
		mRenderItem(nullptr)
	{
		mVertexFormat = RenderManager::getInstance().getVertexFormat();
	}

	RotatingSkin::~RotatingSkin()
	{
	}

	void RotatingSkin::setAngle(float _angle)
	{
		mAngle = _angle;
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::setCenter(const IntPoint& _center)
	{
		mCenterPos = _center;
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
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
		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
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

	void RotatingSkin::_setAlign(const IntSize& _oldsize/*, bool _update*/)
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

	void RotatingSkin::_updateView()
	{
		mEmptyView = ((0 >= _getViewWidth()) || (0 >= _getViewHeight()));

		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
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

		for (int i = 1; i < GEOMETRY_VERTICIES_TOTAL_COUNT - 1; ++i)
		{
			verticies[3*i - 3].set(mResultVerticiesPos[0].left, mResultVerticiesPos[0].top, vertex_z, mResultVerticiesUV[0].left, mResultVerticiesUV[0].top, mCurrentColour);
			verticies[3*i - 2].set(mResultVerticiesPos[i].left, mResultVerticiesPos[i].top, vertex_z, mResultVerticiesUV[i].left, mResultVerticiesUV[i].top, mCurrentColour);
			verticies[3*i - 1].set(mResultVerticiesPos[i+1].left, mResultVerticiesPos[i+1].top, vertex_z, mResultVerticiesUV[i+1].left, mResultVerticiesUV[i+1].top, mCurrentColour);
		}

		mRenderItem->setLastVertexCount((GEOMETRY_VERTICIES_TOTAL_COUNT - 2) * 3);
	}

	void RotatingSkin::_setColour(const Colour& _value)
	{
		uint32 colour = texture_utility::toColourARGB(_value);
		texture_utility::convertColour(colour, mVertexFormat);
		mCurrentColour = (colour & 0x00FFFFFF) | (mCurrentColour & 0xFF000000);

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::setStateData(IStateInfo* _data)
	{
		_setUVSet(_data->castType<SubSkinStateInfo>()->getRect());
	}

	void RotatingSkin::_setUVSet(const FloatRect& _rect)
	{
		mCurrentTexture = _rect;

		mGeometryOutdated = true;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	inline float len(float x, float y)
	{
		return sqrt(x * x + y * y);
	}

	void RotatingSkin::_rebuildGeometry()
	{
		/*
			0 1
			3 2
		*/
#ifndef M_PI
		const float M_PI = 3.141593f;
#endif

		float width_base = (float)mCurrentCoord.width;
		float height_base = (float)mCurrentCoord.height;

		// calculate original unrotated angles of uncropped rectangle verticies: between axis and line from center of rotation to vertex)
		float baseAngles[RECT_VERTICIES_COUNT];
		baseAngles[0] = atan2(                (float)mCenterPos.left,                 (float)mCenterPos.top) + M_PI / 2;
		baseAngles[1] = atan2( - width_base + (float)mCenterPos.left,                 (float)mCenterPos.top) + M_PI / 2;
		baseAngles[2] = atan2( - width_base + (float)mCenterPos.left, - height_base + (float)mCenterPos.top) + M_PI / 2;
		baseAngles[3] = atan2(                (float)mCenterPos.left, - height_base + (float)mCenterPos.top) + M_PI / 2;

		// calculate original unrotated distances of uncropped rectangle verticies: between center of rotation and vertex)
		float baseDistances[RECT_VERTICIES_COUNT];
		baseDistances[0] = len(                (float)mCenterPos.left,                 (float)mCenterPos.top);
		baseDistances[1] = len( - width_base + (float)mCenterPos.left,                 (float)mCenterPos.top);
		baseDistances[2] = len( - width_base + (float)mCenterPos.left, - height_base + (float)mCenterPos.top);
		baseDistances[3] = len(                (float)mCenterPos.left, - height_base + (float)mCenterPos.top);


		// calculate rotated postions of uncropped rectangle verticies (relative to parent)
		FloatPoint baseVerticiesPos[RECT_VERTICIES_COUNT];

		int offsetX = /*mCurrentCoord.left +*/ mCenterPos.left;
		int offsetY = /*mCurrentCoord.top +*/ mCenterPos.top;

		for (int i = 0; i < RECT_VERTICIES_COUNT; ++i)
		{
			baseVerticiesPos[i].left = offsetX + cos(-mAngle + baseAngles[i]) * baseDistances[i];
			baseVerticiesPos[i].top = offsetY - sin(-mAngle + baseAngles[i]) * baseDistances[i];
		}

		// base texture coordinates
		FloatPoint baseVerticiesUV[RECT_VERTICIES_COUNT] =
		{
			FloatPoint(mCurrentTexture.left, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.bottom),
			FloatPoint(mCurrentTexture.left, mCurrentTexture.bottom)
		};

		// now we have rotated uncropped rectangle verticies coordinates

		// --------- here the cropping starts ---------

		// now we are going to calculate vertices of resulting figure

		// no parent - no cropping
		int size = RECT_VERTICIES_COUNT;
		if (nullptr == mCroppedParent->getCroppedParent())
		{
			for (int i = 0; i < RECT_VERTICIES_COUNT; ++i)
			{
				mResultVerticiesPos[i] = baseVerticiesPos[i];
				mResultVerticiesUV[i] = baseVerticiesUV[i];
			}
		}
		else
		{
			size = _cropRotatedRectangle(baseVerticiesPos);

			// calculate texture coordinates
			FloatPoint v0 = baseVerticiesUV[1] - baseVerticiesUV[0];
			FloatPoint v1 = baseVerticiesUV[3] - baseVerticiesUV[0];
			for (int i = 0; i < GEOMETRY_VERTICIES_TOTAL_COUNT; ++i)
			{
				if (i <= size - 1)
				{
					FloatPoint point = _getPositionInsideRect(mResultVerticiesPos[i], baseVerticiesPos[0], baseVerticiesPos[1], baseVerticiesPos[3]);
					mResultVerticiesUV[i] = FloatPoint(
						baseVerticiesUV[0].left + point.left * v0.left + point.top * v1.left,
						baseVerticiesUV[0].top  + point.left * v0.top  + point.top * v1.top
					);
				}
				else
				{
					// all unused verticies is equal to last used
					mResultVerticiesUV[i] = mResultVerticiesUV[size - 1];
				}
			}
		}


		// now calculate widget base offset and then resulting position in screen coordinates
		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();
		float vertex_left_base = ((info.pixScaleX * (float)(mCroppedParent->getAbsoluteLeft()) + info.hOffset) * 2) - 1;
		float vertex_top_base = -(((info.pixScaleY * (float)(mCroppedParent->getAbsoluteTop()) + info.vOffset) * 2) - 1);

		for (int i = 0; i < GEOMETRY_VERTICIES_TOTAL_COUNT; ++i)
		{
			if (i <= size - 1)
			{
				mResultVerticiesPos[i].left = vertex_left_base + mResultVerticiesPos[i].left * info.pixScaleX * 2;
				mResultVerticiesPos[i].top = vertex_top_base + mResultVerticiesPos[i].top * info.pixScaleY * -2;
			}
			else
			{
				// all unused verticies is equal to last used
				mResultVerticiesPos[i] = mResultVerticiesPos[size - 1];
			}
		}
	}

	size_t RotatingSkin::_cropRotatedRectangle(FloatPoint* _baseVerticiesPos)
	{
		std::vector<FloatPoint> resultVerticiesPos;
		resultVerticiesPos.resize(RECT_VERTICIES_COUNT);
		for (int i = 0; i < RECT_VERTICIES_COUNT; ++i)
		{
			resultVerticiesPos[i] = _baseVerticiesPos[i];
		}

		ICroppedRectangle* parent = mCroppedParent->getCroppedParent();
		_cropRotatedRectangleSide(resultVerticiesPos, parent->_getMarginLeft() - mCroppedParent->getLeft(), Left);
		_cropRotatedRectangleSide(resultVerticiesPos, parent->_getMarginLeft() + parent->_getViewWidth() - mCroppedParent->getLeft(), Right);
		_cropRotatedRectangleSide(resultVerticiesPos, parent->_getMarginTop() - mCroppedParent->getTop(), Top);
		_cropRotatedRectangleSide(resultVerticiesPos, parent->_getMarginTop() + parent->_getViewHeight() - mCroppedParent->getTop(), Bottom);

		for (size_t i = 0; i < resultVerticiesPos.size(); ++i)
		{
			mResultVerticiesPos[i] = resultVerticiesPos[i];
		}

		return resultVerticiesPos.size();
	}

	void RotatingSkin::_cropRotatedRectangleSide(std::vector<FloatPoint>& _verticies, int _sideCoord, Side _side)
	{
		std::vector<FloatPoint> newVerticies;
		int invert = (_side == Right || _side == Bottom) ? -1 : 1;
		for (size_t i = 0; i < _verticies.size(); ++i)
		{
			FloatPoint& v0 = _verticies[i];
			FloatPoint& v1 = _verticies[(i+1)%_verticies.size()];
			switch (_side)
			{
			case Left:
			case Right:
				// both inside
				if (invert* v0.left >= invert* _sideCoord && invert* v1.left >= invert * _sideCoord)
					newVerticies.push_back(v0);
				// intersect side (1st vertex in)
				else if (invert* v0.left >= invert * _sideCoord && invert * v1.left < invert * _sideCoord)
				{
					newVerticies.push_back(v0);
					float c = (v0.left - _sideCoord) / (_sideCoord - v1.left);
					newVerticies.push_back(FloatPoint((float)_sideCoord, (v0.top + c * v1.top) / (c + 1)));
				}
				// intersect side (2nd vertex in)
				else if (invert* v0.left <= invert * _sideCoord && invert * v1.left > invert * _sideCoord)
				{
					float c = (v0.left - _sideCoord) / (_sideCoord - v1.left);
					newVerticies.push_back(FloatPoint((float)_sideCoord, (v0.top + c * v1.top) / (c + 1)));
				}
				// else don't add any verticies
				break;
			case Top:
			case Bottom:
				// both inside
				if (invert* v0.top >= invert* _sideCoord && invert* v1.top >= invert * _sideCoord)
					newVerticies.push_back(v0);
				// intersect side (1st vertex in)
				else if (invert* v0.top >= invert * _sideCoord && invert * v1.top < invert * _sideCoord)
				{
					newVerticies.push_back(v0);
					float c = (v0.top - _sideCoord) / (_sideCoord - v1.top);
					newVerticies.push_back(FloatPoint((v0.left + c * v1.left) / (c + 1), (float)_sideCoord));
				}
				// intersect side (2nd vertex in)
				else if (invert* v0.top <= invert * _sideCoord && invert * v1.top > invert * _sideCoord)
				{
					float c = (v0.top - _sideCoord) / (_sideCoord - v1.top);
					newVerticies.push_back(FloatPoint((v0.left + c * v1.left) / (c + 1), (float)_sideCoord));
				}
				// else don't add any verticies
				break;
			}
		}

		_verticies = newVerticies;
	}

	FloatPoint RotatingSkin::_getPositionInsideRect(const FloatPoint& _point, const FloatPoint& _corner0, const FloatPoint& _corner1, const FloatPoint& _corner2)
	{
		FloatPoint result;

		FloatPoint point = _point - _corner0;
		FloatPoint dirX = _corner1 - _corner0;
		FloatPoint dirY = _corner2 - _corner0;

		float div = dirX.left * dirY.top - dirX.top * dirY.left;
		return FloatPoint(
			(point.top * dirX.left - point.left * dirX.top) / div,
			(point.left * dirY.top - point.top * dirY.left) / div
		);
	}

} // namespace MyGUI
