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
#include "MyGUI_LanguageManager.h"
#include "MyGUI_CommonStateInfo.h"

namespace MyGUI
{

	#define MYGUI_CONVERT_COLOUR(colour, format) \
		if (mVertexFormat == VertexColourType::ColourABGR) \
		{ \
			colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00); \
		}

	RawRect::RawRect(/*const SubWidgetInfo &_info, ICroppedRectangle * _parent*/) :
		SubSkin(/*_info, _parent*/),
		mRectTextureLT(FloatPoint(0, 0)),
		mRectTextureRT(FloatPoint(1, 0)),
		mRectTextureLB(FloatPoint(0, 1)),
		mRectTextureRB(FloatPoint(1, 1)),
		mColourLT(Colour::White),
		mColourRT(Colour::White),
		mColourLB(Colour::White),
		mColourRB(Colour::White),
		mRenderColourLT(0xFFFFFFFF),
		mRenderColourRT(0xFFFFFFFF),
		mRenderColourLB(0xFFFFFFFF),
		mRenderColourRB(0xFFFFFFFF)
	{
		mVertexFormat = RenderManager::getInstance().getVertexFormat();
	}

	RawRect::~RawRect()
	{
	}

	void RawRect::setAlpha(float _alpha)
	{
		mCurrentAlpha = ((uint8)(_alpha*255) << 24);

		mRenderColourLT = mCurrentAlpha | (mRenderColourLT & 0x00FFFFFF);
		mRenderColourRT = mCurrentAlpha | (mRenderColourRT & 0x00FFFFFF);
		mRenderColourLB = mCurrentAlpha | (mRenderColourLB & 0x00FFFFFF);
		mRenderColourRB = mCurrentAlpha | (mRenderColourRB & 0x00FFFFFF);

		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void RawRect::setRectColour(const Colour& _colourLT, const Colour& _colourRT, const Colour& _colourLB, const Colour& _colourRB)
	{
		mColourLT = _colourLT;
		mRenderColourLT = mColourLT.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourLT, mVertexFormat);
		mRenderColourLT = mCurrentAlpha | (mRenderColourLT & 0x00FFFFFF);

		mColourRT = _colourRT;
		mRenderColourRT = mColourRT.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourRT, mVertexFormat);
		mRenderColourRT = mCurrentAlpha | (mRenderColourRT & 0x00FFFFFF);

		mColourLB = _colourLB;
		mRenderColourLB = mColourLB.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourLB, mVertexFormat);
		mRenderColourLB = mCurrentAlpha | (mRenderColourLB & 0x00FFFFFF);

		mColourRB = _colourRB;
		mRenderColourRB = mColourRB.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourRB, mVertexFormat);
		mRenderColourRB = mCurrentAlpha | (mRenderColourRB & 0x00FFFFFF);

		if (nullptr != mNode) mNode->outOfDate(mRenderItem);
	}

	void RawRect::setRectTexture(const FloatPoint & _pointLT, const FloatPoint & _pointRT, const FloatPoint & _pointLB, const FloatPoint & _pointRB)
	{
		mRectTextureLT = _pointLT;
		mRectTextureRT = _pointRT;
		mRectTextureLB = _pointLB;
		mRectTextureRB = _pointRB;
	}

	void RawRect::doRender()
	{
		if (!mVisible || mEmptyView) return;

		VertexQuad* quad = (VertexQuad*)mRenderItem->getCurrentVertextBuffer();

		const RenderTargetInfo& info = mRenderItem->getRenderTarget()->getInfo();

		float vertex_z = info.maximumDepth;

		float vertex_left = ((info.pixScaleX * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft() - info.leftOffset) + info.hOffset) * 2) - 1;
		float vertex_right = vertex_left + (info.pixScaleX * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((info.pixScaleY * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop() - info.topOffset) + info.vOffset) * 2) - 1);
		float vertex_bottom = vertex_top - (info.pixScaleY * (float)mCurrentCoord.height * 2);

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
