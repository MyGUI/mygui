/*!
	@file
	@author		George Evmenov
	@date		05/2009
	@module
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

namespace MyGUI
{

	RotatingSkin::RotatingSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent),
		mAngle(0.)
	{
	}

	RotatingSkin::~RotatingSkin()
	{
	}

	void RotatingSkin::setAngle(float _angle)
	{
		mAngle = _angle;
		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void RotatingSkin::setCenter(const IntPoint &_center, bool _local)
	{
		mCenterPos = _center;
		mLocalCenter = _local;
		recalculateAngles();
		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	IntPoint RotatingSkin::getCenter(bool _local) const
	{
		return mCenterPos + (_local ? IntPoint() : mCroppedParent->getAbsolutePosition());
	}

	void RotatingSkin::doRender()
	{
		if ((false == mVisible) || mEmptyView) return;

		VertexQuad* quad = (VertexQuad*)mRenderItem->getCurrentVertextBuffer();

		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();

		float vertex_z = info.maximumDepth;

		float vertex_left_base = ((info.pixScaleX * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft() + mCenterPos.left) + info.hOffset) * 2) - 1;
		float vertex_top_base = -(((info.pixScaleY * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() + mCenterPos.top) + info.vOffset) * 2) - 1);

		// FIXME: do it only when size changes
		recalculateAngles();

		float vertex_left = vertex_left_base + cos(mAngle + mBaseAngles[0]) * mBaseDistances[0] * info.pixScaleX * -2;
		float vertex_right = vertex_left_base + cos(mAngle + mBaseAngles[2]) * mBaseDistances[2] * info.pixScaleX * -2;
		float vertex_top = vertex_top_base + sin(mAngle + mBaseAngles[0]) * mBaseDistances[0] * info.pixScaleY * -2;
		float vertex_bottom = vertex_top_base + sin(mAngle + mBaseAngles[2]) * mBaseDistances[2] * info.pixScaleY * -2;

		quad->set(
			vertex_left,
			vertex_top,
			vertex_right,
			vertex_bottom,
			vertex_z,
			mCurrentTexture.left,
			mCurrentTexture.top,
			mCurrentTexture.right,
			mCurrentTexture.bottom,
			mCurrentAlpha
			);

		mRenderItem->setLastVertexCount(VertexQuad::VertexCount);

		// first triangle - left top
		/*_vertex[0].x = vertex_left_base + cos(mAngle + mBaseAngles[0]) * mBaseDistances[0] * info.pixScaleX * -2;
		_vertex[0].y = vertex_top_base + sin(mAngle + mBaseAngles[0]) * mBaseDistances[0] * info.pixScaleY * -2;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mCurrentAlpha;
		_vertex[0].u = mCurrentTexture.left;
		_vertex[0].v = mCurrentTexture.top;

		// first triangle - left bottom
		_vertex[1].x = vertex_left_base + cos(mAngle + mBaseAngles[1]) * mBaseDistances[1] * info.pixScaleX * -2;
		_vertex[1].y = vertex_top_base + sin(mAngle + mBaseAngles[1]) * mBaseDistances[1] * info.pixScaleY * -2;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mCurrentAlpha;
		_vertex[1].u = mCurrentTexture.left;
		_vertex[1].v = mCurrentTexture.bottom;

		// first triangle - right top
		_vertex[2].x = vertex_left_base + cos(mAngle + mBaseAngles[3]) * mBaseDistances[3] * info.pixScaleX * -2;
		_vertex[2].y = vertex_top_base + sin(mAngle + mBaseAngles[3]) * mBaseDistances[3] * info.pixScaleY * -2;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mCurrentAlpha;
		_vertex[2].u = mCurrentTexture.right;
		_vertex[2].v = mCurrentTexture.top;

		// second triangle - right top
		_vertex[3] = _vertex[2];

		// second triangle = left bottom
		_vertex[4] = _vertex[1];

		// second triangle - right botton
		_vertex[5].x = vertex_left_base + cos(mAngle + mBaseAngles[2]) * mBaseDistances[2] * info.pixScaleX * -2;
		_vertex[5].y = vertex_top_base + sin(mAngle + mBaseAngles[2]) * mBaseDistances[2] * info.pixScaleY * -2;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mCurrentAlpha;
		_vertex[5].u = mCurrentTexture.right;
		_vertex[5].v = mCurrentTexture.bottom;*/
	}

	StateInfo * RotatingSkin::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		return SubSkin::createStateData(_node, _root, _version);
	}

	inline float len(float x, float y) { return sqrt(x*x + y*y); }

	void RotatingSkin::recalculateAngles()
	{
#ifndef M_PI
			const float M_PI = 3.141593;
#endif
		// FIXME mLocalCenter ignored
		float left_base = 0;
		float top_base = 0;

		if (!mLocalCenter)
		{
			left_base = (float)mCurrentCoord.width;
			top_base = (float)mCurrentCoord.height;
		}

		float width_base = (float)mCurrentCoord.width;
		float height_base = (float)mCurrentCoord.height;

		mBaseAngles[0] = atan2((float)           - mCenterPos.left,             - mCenterPos.top);
		mBaseAngles[1] = atan2((float)           - mCenterPos.left, height_base - mCenterPos.top);
		mBaseAngles[2] = atan2((float)width_base - mCenterPos.left, height_base - mCenterPos.top);
		mBaseAngles[3] = atan2((float)width_base - mCenterPos.left,             - mCenterPos.top);

		mBaseDistances[0] = len((float)           - mCenterPos.left,             - mCenterPos.top);
		mBaseDistances[1] = len((float)           - mCenterPos.left, height_base - mCenterPos.top);
		mBaseDistances[2] = len((float)width_base - mCenterPos.left, height_base - mCenterPos.top);
		mBaseDistances[3] = len((float)width_base - mCenterPos.left,             - mCenterPos.top);

	}

} // namespace MyGUI
