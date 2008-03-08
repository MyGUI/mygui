/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/
#include "MyGUI_ColourRect.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"

namespace MyGUI
{

	const size_t COLOURRECT_COUNT_VERTEX = VERTEX_IN_QUAD;

	ColourRect::ColourRect(const SubWidgetInfo &_info, CroppedRectanglePtr _parent) :
		SubSkin(_info, _parent),
		mRenderColourLeftTop(0xFFFFFFFF),
		mRenderColourRightTop(0xFFFFFFFF),
		mRenderColourRightBottom(0xFFFFFFFF),
		mRenderColourLeftBottom(0xFFFFFFFF),
		mColourLeftTop(Ogre::ColourValue::White),
		mColourRightTop(Ogre::ColourValue::White),
		mColourRightBottom(Ogre::ColourValue::White),
		mColourLeftBottom(Ogre::ColourValue::White)
	{
		mRenderColourLeftTop = 0xFF000000;
		mRenderColourRightTop = 0xFFFF00FF;
		mRenderColourRightBottom = 0xFFFFFFFF;
		mRenderColourLeftBottom = 0xFF000000;
	}

	ColourRect::~ColourRect()
	{
	}

	void ColourRect::setAlpha(float _alpha)
	{
		mCurrentAlpha = ((uint8)(_alpha*255) << 24);

		mRenderColourLeftTop = mCurrentAlpha | (mRenderColourLeftTop & 0x00FFFFFF);
		mRenderColourRightTop = mCurrentAlpha | (mRenderColourRightTop & 0x00FFFFFF);
		mRenderColourRightBottom = mCurrentAlpha | (mRenderColourRightBottom & 0x00FFFFFF);
		mRenderColourLeftBottom = mCurrentAlpha | (mRenderColourLeftBottom & 0x00FFFFFF);

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void ColourRect::setColourAlign(const Ogre::ColourValue & _colour, Align _align)
	{
		if (IS_ALIGN_RIGHT_TOP(_align)) {
			if (mColourRightTop == _colour) return;
			mColourRightTop = _colour;
			Ogre::Root::getSingleton().convertColourValue(mColourRightTop, &mRenderColourRightTop);

			mRenderColourRightTop = mCurrentAlpha | (mRenderColourRightTop & 0x00FFFFFF);
		}
		else if (IS_ALIGN_RIGHT_BOTTOM(_align)) {
			if (mColourRightBottom == _colour) return;
			mColourRightBottom = _colour;
			Ogre::Root::getSingleton().convertColourValue(mColourRightBottom, &mRenderColourRightBottom);

			mRenderColourRightBottom = mCurrentAlpha | (mRenderColourRightBottom & 0x00FFFFFF);
		}
		else if (IS_ALIGN_LEFT_BOTTOM(_align)) {
			if (mColourLeftBottom == _colour) return;
			mColourLeftBottom = _colour;
			Ogre::Root::getSingleton().convertColourValue(mColourLeftBottom, &mRenderColourLeftBottom);

			mRenderColourLeftBottom = mCurrentAlpha | (mRenderColourLeftBottom & 0x00FFFFFF);
		}
		else {
			if (mColourLeftTop == _colour) return;
			mColourLeftTop = _colour;
			Ogre::Root::getSingleton().convertColourValue(mColourLeftTop, &mRenderColourLeftTop);

			mRenderColourLeftTop = mCurrentAlpha | (mRenderColourLeftTop & 0x00FFFFFF);
		}

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	const Ogre::ColourValue & ColourRect::getColourAlign(Align _align)
	{
		if (IS_ALIGN_RIGHT_TOP(_align)) return mColourRightTop;
		else if (IS_ALIGN_RIGHT_BOTTOM(_align)) return mColourRightBottom;
		else if (IS_ALIGN_LEFT_BOTTOM(_align)) return mColourLeftBottom;
		return mColourLeftTop;
	}

	size_t ColourRect::_drawItem(Vertex * _vertex, bool _update)
	{
		if ((false == mShow) || mEmptyView) return 0;

		float vertex_z = mManager->getMaximumDepth();

		float vertex_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float vertex_bottom = vertex_top - (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);

		// first triangle - left top
		_vertex[0].x = vertex_left;
		_vertex[0].y = vertex_top;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mRenderColourLeftTop;
		_vertex[0].u = mCurrentTexture.left;
		_vertex[0].v = mCurrentTexture.top;
		

		// first triangle - left bottom
		_vertex[1].x = vertex_left;
		_vertex[1].y = vertex_bottom;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mRenderColourLeftBottom;
		_vertex[1].u = mCurrentTexture.left;
		_vertex[1].v = mCurrentTexture.bottom;

		// first triangle - right top
		_vertex[2].x = vertex_right;
		_vertex[2].y = vertex_top;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mRenderColourRightTop;
		_vertex[2].u = mCurrentTexture.right;
		_vertex[2].v = mCurrentTexture.top;

		// second triangle - right top
		_vertex[3].x = vertex_right;
		_vertex[3].y = vertex_top;
		_vertex[3].z = vertex_z;
		_vertex[3].colour = mRenderColourRightTop;
		_vertex[3].u = mCurrentTexture.right;
		_vertex[3].v = mCurrentTexture.top;

		// second triangle = left bottom
		_vertex[4].x = vertex_left;
		_vertex[4].y = vertex_bottom;
		_vertex[4].z = vertex_z;
		_vertex[4].colour = mRenderColourLeftBottom;
		_vertex[4].u = mCurrentTexture.left;
		_vertex[4].v = mCurrentTexture.bottom;

		// second triangle - right botton
		_vertex[5].x = vertex_right;
		_vertex[5].y = vertex_bottom;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mRenderColourRightBottom;
		_vertex[5].u = mCurrentTexture.right;
		_vertex[5].v = mCurrentTexture.bottom;

		return COLOURRECT_COUNT_VERTEX;
	}

} // namespace MyGUI
