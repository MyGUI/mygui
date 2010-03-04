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

namespace MyGUI
{

	RotatingSkin::RotatingSkin() :
		mAngle(0.0f),
		mLocalCenter(false),
		mEmptyView(false),
		mNode(nullptr),
		mRenderItem(nullptr)
	{
	}

	RotatingSkin::~RotatingSkin()
	{
	}

	void RotatingSkin::setAngle(float _angle)
	{
		mAngle = _angle;
		recalculateAngles();

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::setCenter(const IntPoint &_center, bool _local)
	{
		mCenterPos = _center;
		mLocalCenter = _local;
		recalculateAngles();

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

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::setAlpha(float _alpha)
	{
		uint8_t alpha = (uint8_t)(_alpha*255);
		if (alpha == mCurrentColour.data.alpha)
			return;

		mCurrentColour.data.alpha = alpha;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::_correctView()
	{
		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::_setAlign(const IntSize& _oldsize, bool _update)
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

		recalculateAngles();

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::createDrawItem(ITexture* _texture, ILayerNode * _node)
	{
		MYGUI_ASSERT(!mRenderItem, "mRenderItem must be nullptr");

		mNode = _node;
		mRenderItem = mNode->addToRenderItem(_texture, this);
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
		mCurrentColour.data.colour = ColourARGB::fromColour(_value, mVertexFormat).data.colour;

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

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	inline float len(float x, float y) { return sqrt(x*x + y*y); }

	void RotatingSkin::recalculateAngles()
	{
		/*
			0 1
			3 2
		*/
#ifndef M_PI
		const float M_PI = 3.141593f;
#endif
		// FIXME? mLocalCenter ignored
		float left_base = 0;
		float top_base = 0;

		if (!mLocalCenter)
		{
			left_base = (float)mCurrentCoord.width;
			top_base = (float)mCurrentCoord.height;
		}

		float width_base = (float)mCurrentCoord.width;
		float height_base = (float)mCurrentCoord.height;

		// calculate original unrotated angles of uncropped rectangle verticies: between axis and line from center of rotation to vertex)
		float baseAngles[RECT_VERTICIES_COUNT];
		baseAngles[0] = atan2((float)           - mCenterPos.left,             - mCenterPos.top) + M_PI/2;
		baseAngles[1] = atan2((float)width_base - mCenterPos.left,             - mCenterPos.top) + M_PI/2;
		baseAngles[2] = atan2((float)width_base - mCenterPos.left, height_base - mCenterPos.top) + M_PI/2;
		baseAngles[3] = atan2((float)           - mCenterPos.left, height_base - mCenterPos.top) + M_PI/2;

		// calculate original unrotated distances of uncropped rectangle verticies: between center of rotation and vertex)
		float baseDistances[RECT_VERTICIES_COUNT];
		baseDistances[0] = len((float)           - mCenterPos.left,             - mCenterPos.top);
		baseDistances[1] = len((float)width_base - mCenterPos.left,             - mCenterPos.top);
		baseDistances[2] = len((float)width_base - mCenterPos.left, height_base - mCenterPos.top);
		baseDistances[3] = len((float)           - mCenterPos.left, height_base - mCenterPos.top);


		// calculate rotated postions of uncropped rectangle verticies (relative to parent)
		FloatPoint baseVerticiesPos[RECT_VERTICIES_COUNT];
		if (mRenderItem && mRenderItem->getRenderTarget())
		{
			float offsetX = mCurrentCoord.left + mCenterPos.left;
			float offsetY = mCurrentCoord.top + mCenterPos.top;

			for (int i = 0; i < RECT_VERTICIES_COUNT; ++i)
			{
				baseVerticiesPos[i].left = offsetX + cos(-mAngle + baseAngles[i]) * baseDistances[i];
				baseVerticiesPos[i].top = offsetY - sin(-mAngle + baseAngles[i]) * baseDistances[i];
			}
		}

		// base texture coordinates
		FloatPoint baseVerticiesUV[RECT_VERTICIES_COUNT] = {
			FloatPoint(mCurrentTexture.left, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.top),
			FloatPoint(mCurrentTexture.right, mCurrentTexture.bottom),
			FloatPoint(mCurrentTexture.left, mCurrentTexture.bottom)
		};

		// now we have rotated uncropped rectangle verticies coordinates
		// ----- here the cropping starts

		// now we are going to calculate vertices of resulting figure
		for (int i = 0; i < RECT_VERTICIES_COUNT; ++i)
		{
			mResultVerticiesPos[2*i] = baseVerticiesPos[i];
			mResultVerticiesUV[2*i] = baseVerticiesUV[i];

			mResultVerticiesPos[2*i + 1] = baseVerticiesPos[i];
			mResultVerticiesUV[2*i + 1] = baseVerticiesUV[i];
		}


		// now calculate widget base offset in screen coordinates
		if (mRenderItem && mRenderItem->getRenderTarget())
		{
			const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();
			float vertex_left_base = ((info.pixScaleX * (float)(mCroppedParent->getAbsoluteLeft()) + info.hOffset) * 2) - 1;
			float vertex_top_base = -(((info.pixScaleY * (float)(mCroppedParent->getAbsoluteTop()) + info.vOffset) * 2) - 1);

			for (int i = 0; i < GEOMETRY_VERTICIES_TOTAL_COUNT; ++i)
			{
				mResultVerticiesPos[i].left = vertex_left_base + mResultVerticiesPos[i].left * info.pixScaleX * 2;
				mResultVerticiesPos[i].top = vertex_top_base + mResultVerticiesPos[i].top * info.pixScaleY * -2;
			}
		}
	}

} // namespace MyGUI
