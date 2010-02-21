/*!
	@file
	@author		Albert Semenov
	@date		03/2008
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
#include "MyGUI_RawRect.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_CommonStateInfo.h"

namespace MyGUI
{

	RawRect::RawRect() :
		mRectTextureLT(FloatPoint(0, 0)),
		mRectTextureRT(FloatPoint(1, 0)),
		mRectTextureLB(FloatPoint(0, 1)),
		mRectTextureRB(FloatPoint(1, 1))
	{
		mRenderColourLT.value = ColourARGB::White;
		mRenderColourRT.value = ColourARGB::White;
		mRenderColourLB.value = ColourARGB::White;
		mRenderColourRB.value = ColourARGB::White;
	}

	RawRect::~RawRect()
	{
	}

	void RawRect::setAlpha(float _alpha)
	{
		byte alpha = (byte)(_alpha * 255);

		mRenderColourLT.data.alpha = alpha;
		mRenderColourRT.data.alpha = alpha;
		mRenderColourLB.data.alpha = alpha;
		mRenderColourRB.data.alpha = alpha;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RawRect::setRectColour(const Colour& _colourLT, const Colour& _colourRT, const Colour& _colourLB, const Colour& _colourRB)
	{
		mRenderColourLT.data.colour = ColourARGB::fromColour(_colourLT, mVertexFormat).data.colour;
		mRenderColourRT.data.colour = ColourARGB::fromColour(_colourRT, mVertexFormat).data.colour;
		mRenderColourLB.data.colour = ColourARGB::fromColour(_colourLB, mVertexFormat).data.colour;
		mRenderColourRB.data.colour = ColourARGB::fromColour(_colourRB, mVertexFormat).data.colour;

		if (nullptr != mNode)
			mNode->outOfDate(mRenderItem);
	}

	void RawRect::setRectTexture(const FloatPoint& _pointLT, const FloatPoint& _pointRT, const FloatPoint& _pointLB, const FloatPoint& _pointRB)
	{
		mRectTextureLT = _pointLT;
		mRectTextureRT = _pointRT;
		mRectTextureLB = _pointLB;
		mRectTextureRB = _pointRB;
	}

	void RawRect::doRender()
	{
		if (!mVisible || mEmptyView)
			return;

		Vertex* _vertex = mRenderItem->getCurrentVertextBuffer();

		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();

		float vertex_z = info.maximumDepth;

		float vertex_left = ((info.pixScaleX * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft() - info.leftOffset) + info.hOffset) * 2) - 1;
		float vertex_right = vertex_left + (info.pixScaleX * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((info.pixScaleY * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() - info.topOffset) + info.vOffset) * 2) - 1);
		float vertex_bottom = vertex_top - (info.pixScaleY * (float)mCurrentCoord.height * 2);

		// first triangle - left top
		_vertex[0].x = vertex_left;
		_vertex[0].y = vertex_top;
		_vertex[0].z = vertex_z;
		_vertex[0].colour = mRenderColourLT;
		_vertex[0].u = mRectTextureLT.left;
		_vertex[0].v = mRectTextureLT.top;

		// first triangle - left bottom
		_vertex[1].x = vertex_left;
		_vertex[1].y = vertex_bottom;
		_vertex[1].z = vertex_z;
		_vertex[1].colour = mRenderColourLB;
		_vertex[1].u = mRectTextureLB.left;
		_vertex[1].v = mRectTextureLB.top;

		// first triangle - right top
		_vertex[2].x = vertex_right;
		_vertex[2].y = vertex_top;
		_vertex[2].z = vertex_z;
		_vertex[2].colour = mRenderColourRT;
		_vertex[2].u = mRectTextureRT.left;
		_vertex[2].v = mRectTextureRT.top;

		// second triangle - right top
		_vertex[3].x = vertex_right;
		_vertex[3].y = vertex_top;
		_vertex[3].z = vertex_z;
		_vertex[3].colour = mRenderColourRT;
		_vertex[3].u = mRectTextureRT.left;
		_vertex[3].v = mRectTextureRT.top;

		// second triangle = left bottom
		_vertex[4].x = vertex_left;
		_vertex[4].y = vertex_bottom;
		_vertex[4].z = vertex_z;
		_vertex[4].colour = mRenderColourLB;
		_vertex[4].u = mRectTextureLB.left;
		_vertex[4].v = mRectTextureLB.top;

		// second triangle - right botton
		_vertex[5].x = vertex_right;
		_vertex[5].y = vertex_bottom;
		_vertex[5].z = vertex_z;
		_vertex[5].colour = mRenderColourRB;
		_vertex[5].u = mRectTextureRB.left;
		_vertex[5].v = mRectTextureRB.top;

		mRenderItem->setLastVertexCount(VertexQuad::VertexCount);
	}

	void RawRect::setStateData(IStateInfo * _data)
	{
		SubSkinStateInfo* data = _data->castType<SubSkinStateInfo>();
		const FloatRect& rect = data->getRect();
		mRectTextureLT.set(rect.left, rect.top);
		mRectTextureRT.set(rect.right, rect.top);
		mRectTextureLB.set(rect.left, rect.bottom);
		mRectTextureRB.set(rect.right, rect.bottom);
	}

} // namespace MyGUI
