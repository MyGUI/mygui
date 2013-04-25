/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_RotatingSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_CommonStateInfo.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_GeometryUtility.h"

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

	void RotatingSkin::_setAlign(const IntSize& _oldsize)
	{
		// необходимо разобраться
		bool need_update = true;

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
		if (!mVisible || mEmptyView)
			return;

		Vertex* verticies = mRenderItem->getCurrentVertexBuffer();

		float vertex_z = mNode->getNodeDepth();

		if (mGeometryOutdated)
		{
			_rebuildGeometry();
			mGeometryOutdated = false;
		}

		for (int i = 1; i < GEOMETRY_VERTICIES_TOTAL_COUNT - 1; ++i)
		{
			verticies[3 * i - 3].set(mResultVerticiesPos[0].left, mResultVerticiesPos[0].top, vertex_z, mResultVerticiesUV[0].left, mResultVerticiesUV[0].top, mCurrentColour);
			verticies[3 * i - 2].set(mResultVerticiesPos[i].left, mResultVerticiesPos[i].top, vertex_z, mResultVerticiesUV[i].left, mResultVerticiesUV[i].top, mCurrentColour);
			verticies[3 * i - 1].set(mResultVerticiesPos[i + 1].left, mResultVerticiesPos[i + 1].top, vertex_z, mResultVerticiesUV[i + 1].left, mResultVerticiesUV[i + 1].top, mCurrentColour);
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
		baseAngles[0] = atan2((float)mCenterPos.left, (float)mCenterPos.top) + M_PI / 2;
		baseAngles[1] = atan2(- width_base + (float)mCenterPos.left, (float)mCenterPos.top) + M_PI / 2;
		baseAngles[2] = atan2(- width_base + (float)mCenterPos.left, - height_base + (float)mCenterPos.top) + M_PI / 2;
		baseAngles[3] = atan2((float)mCenterPos.left, - height_base + (float)mCenterPos.top) + M_PI / 2;

		// calculate original unrotated distances of uncropped rectangle verticies: between center of rotation and vertex)
		float baseDistances[RECT_VERTICIES_COUNT];
		baseDistances[0] = len((float)mCenterPos.left, (float)mCenterPos.top);
		baseDistances[1] = len(- width_base + (float)mCenterPos.left, (float)mCenterPos.top);
		baseDistances[2] = len(- width_base + (float)mCenterPos.left, - height_base + (float)mCenterPos.top);
		baseDistances[3] = len((float)mCenterPos.left, - height_base + (float)mCenterPos.top);


		// calculate rotated positions of uncropped rectangle verticies (relative to parent)
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

		// now we are going to calculate verticies of resulting figure

		// no parent - no cropping
		size_t size = RECT_VERTICIES_COUNT;
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
			ICroppedRectangle* parent = mCroppedParent->getCroppedParent();

			VectorFloatPoint resultVerticiesPos = geometry_utility::cropPolygon(
				baseVerticiesPos,
				RECT_VERTICIES_COUNT,
				IntCoord(
					parent->_getMarginLeft() - mCroppedParent->getLeft(),
					parent->_getMarginTop() - mCroppedParent->getTop(),
					parent->_getViewWidth(),
					parent->_getViewHeight()));

			for (size_t i = 0; i < resultVerticiesPos.size(); ++i)
			{
				mResultVerticiesPos[i] = resultVerticiesPos[i];
			}

			size = resultVerticiesPos.size();

			// calculate texture coordinates
			FloatPoint v0 = baseVerticiesUV[3] - baseVerticiesUV[0];
			FloatPoint v1 = baseVerticiesUV[1] - baseVerticiesUV[0];
			for (size_t i = 0; i < GEOMETRY_VERTICIES_TOTAL_COUNT; ++i)
			{
				if (i < size)
				{
					FloatPoint point = geometry_utility::getPositionInsideRect(mResultVerticiesPos[i], baseVerticiesPos[0], baseVerticiesPos[1], baseVerticiesPos[3]);
					mResultVerticiesUV[i] = geometry_utility::getUVFromPositionInsideRect(point, v0, v1, baseVerticiesUV[0]);
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

		for (size_t i = 0; i < GEOMETRY_VERTICIES_TOTAL_COUNT; ++i)
		{
			if (i < size)
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

	float RotatingSkin::getAngle() const
	{
		return mAngle;
	}

} // namespace MyGUI
