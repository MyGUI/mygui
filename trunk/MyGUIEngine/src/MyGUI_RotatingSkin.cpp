/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*//*
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
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t SUBSKIN_COUNT_VERTEX = VERTEX_IN_QUAD;

	RotatingSkin::RotatingSkin(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent),
		mAngle(0.)
	{
	}

	RotatingSkin::~RotatingSkin()
	{
	}

	void RotatingSkin::setAngle(double _angle)
	{
		mAngle = _angle;
		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void RotatingSkin::doRender()
	{
		if ((false == mVisible) || mEmptyView) return;

		Vertex* _vertex = mRenderItem->getCurrentVertextBuffer();
		bool _update = mRenderItem->getCurrentUpdate();

		float vertex_z = mManager->getMaximumDepth();
		//vertex_z = 0;

		//float vertex_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		//float vertex_right = vertex_left + (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		//float vertex_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		//float vertex_bottom = vertex_top - (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);

		float vertex_left_base = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float vertex_width_base = (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top_base = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float vertex_height_base = - (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);

		float s = sin(mAngle);
		float c = cos(mAngle);

		// first triangle - left top
		_vertex[0].x = vertex_left_base;
		_vertex[0].y = vertex_top_base;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mCurrentAlpha;
		_vertex[0].u = mCurrentTexture.left;
		_vertex[0].v = mCurrentTexture.top;


		// first triangle - left bottom
		_vertex[1].x = vertex_left_base - s * vertex_height_base;
		_vertex[1].y = vertex_top_base + c * vertex_height_base;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mCurrentAlpha;
		_vertex[1].u = mCurrentTexture.left;
		_vertex[1].v = mCurrentTexture.bottom;

		// first triangle - right top
		_vertex[2].x = vertex_left_base + c * vertex_width_base;
		_vertex[2].y = vertex_top_base + s * vertex_width_base;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mCurrentAlpha;
		_vertex[2].u = mCurrentTexture.right;
		_vertex[2].v = mCurrentTexture.top;

		// second triangle - right top
		_vertex[3].x = vertex_left_base + c * vertex_width_base;
		_vertex[3].y = vertex_top_base + s * vertex_width_base;
		_vertex[3].z = vertex_z;
		_vertex[3].colour = mCurrentAlpha;
		_vertex[3].u = mCurrentTexture.right;
		_vertex[3].v = mCurrentTexture.top;

		// second triangle = left bottom
		_vertex[4].x = vertex_left_base - s * vertex_height_base;
		_vertex[4].y = vertex_top_base + c * vertex_height_base;
		_vertex[4].z = vertex_z;
		_vertex[4].colour = mCurrentAlpha;
		_vertex[4].u = mCurrentTexture.left;
		_vertex[4].v = mCurrentTexture.bottom;

		// FIXME
		float ang = atan2(vertex_height_base, vertex_width_base);
		s = sin(mAngle + ang);
		c = cos(mAngle + ang);
		float diag = sqrt(vertex_width_base*vertex_width_base + vertex_height_base*vertex_height_base);
		// second triangle - right botton
		_vertex[5].x = vertex_left_base + c * diag;
		_vertex[5].y = vertex_top_base + s * diag;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mCurrentAlpha;
		_vertex[5].u = mCurrentTexture.right;
		_vertex[5].v = mCurrentTexture.bottom;

		mRenderItem->setLastVertexCount(SUBSKIN_COUNT_VERTEX);
	}

	StateInfo * RotatingSkin::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		return SubSkin::createStateData(_node, _root, _version);
	}

} // namespace MyGUI
