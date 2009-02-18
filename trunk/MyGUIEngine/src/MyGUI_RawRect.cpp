/*!
	@file
	@author		Albert Semenov
	@date		03/2008
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
#include "MyGUI_RawRect.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"

namespace MyGUI
{

	struct RawRectStateData : public StateInfo
	{
		FloatRect rect;
	};

	const size_t COLOURRECT_COUNT_VERTEX = VERTEX_IN_QUAD;

	#define MYGUI_CONVERT_COLOUR(colour, gl) if (gl) { colour = ((colour&0x00FF0000)>>16)|((colour&0x000000FF)<<16)|(colour&0xFF00FF00); }

	RawRect::RawRect(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent),
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
		// потом перенести
		mRenderGL = (Ogre::VET_COLOUR_ABGR == Ogre::Root::getSingleton().getRenderSystem()->getColourVertexElementType());
	}

	RawRect::~RawRect()
	{
	}

	void RawRect::_setStateData(StateInfo * _data)
	{
		RawRectStateData * data = (RawRectStateData*)_data;
		mRectTextureLT.set(data->rect.left, data->rect.top);
		mRectTextureRT.set(data->rect.right, data->rect.top);
		mRectTextureLB.set(data->rect.left, data->rect.bottom);
		mRectTextureRB.set(data->rect.right, data->rect.bottom);
	}

	void RawRect::setAlpha(float _alpha)
	{
		mCurrentAlpha = ((uint8)(_alpha*255) << 24);

		mRenderColourLT = mCurrentAlpha | (mRenderColourLT & 0x00FFFFFF);
		mRenderColourRT = mCurrentAlpha | (mRenderColourRT & 0x00FFFFFF);
		mRenderColourLB = mCurrentAlpha | (mRenderColourLB & 0x00FFFFFF);
		mRenderColourRB = mCurrentAlpha | (mRenderColourRB & 0x00FFFFFF);

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void RawRect::setRectColour(const Colour& _colourLT, const Colour& _colourRT, const Colour& _colourLB, const Colour& _colourRB)
	{
		mColourLT = _colourLT;
		mRenderColourLT = mColourLT.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourLT, mRenderGL);
		mRenderColourLT = mCurrentAlpha | (mRenderColourLT & 0x00FFFFFF);

		mColourRT = _colourRT;
		mRenderColourRT = mColourRT.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourRT, mRenderGL);
		mRenderColourRT = mCurrentAlpha | (mRenderColourRT & 0x00FFFFFF);

		mColourLB = _colourLB;
		mRenderColourLB = mColourLB.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourLB, mRenderGL);
		mRenderColourLB = mCurrentAlpha | (mRenderColourLB & 0x00FFFFFF);

		mColourRB = _colourRB;
		mRenderColourRB = mColourRB.toColourARGB();
		MYGUI_CONVERT_COLOUR(mRenderColourRB, mRenderGL);
		mRenderColourRB = mCurrentAlpha | (mRenderColourRB & 0x00FFFFFF);

		if (nullptr != mRenderItem) mRenderItem->outOfDate();
	}

	void RawRect::setRectTexture(const FloatPoint & _pointLT, const FloatPoint & _pointRT, const FloatPoint & _pointLB, const FloatPoint & _pointRB)
	{
		mRectTextureLT = _pointLT;
		mRectTextureRT = _pointRT;
		mRectTextureLB = _pointLB;
		mRectTextureRB = _pointRB;
	}

	size_t RawRect::_drawItem(Vertex * _vertex, bool _update)
	{
		if ((false == mVisible) || mEmptyView) return 0;

		float vertex_z = mManager->getMaximumDepth();

		float vertex_left = ((mManager->getPixScaleX() * (float)(mCurrentCoord.left + mCroppedParent->getAbsoluteLeft()) + mManager->getHOffset()) * 2) - 1;
		float vertex_right = vertex_left + (mManager->getPixScaleX() * (float)mCurrentCoord.width * 2);
		float vertex_top = -(((mManager->getPixScaleY() * (float)(mCurrentCoord.top + mCroppedParent->getAbsoluteTop()) + mManager->getVOffset()) * 2) - 1);
		float vertex_bottom = vertex_top - (mManager->getPixScaleY() * (float)mCurrentCoord.height * 2);

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

		return COLOURRECT_COUNT_VERTEX;
	}

	StateInfo * RawRect::createStateData(xml::ElementPtr _node, xml::ElementPtr _root, Version _version)
	{
		std::string texture = _root->findAttribute("texture");

		// поддержка замены тегов в скинах
		if (_version >= Version(1, 1)) {
			texture = LanguageManager::getInstance().replaceTags(texture);
		}

		const IntSize & size = SkinManager::getInstance().getTextureSize(texture);
		RawRectStateData * data = new RawRectStateData();
		const FloatRect & source = FloatRect::parse(_node->findAttribute("offset"));
		data->rect = SkinManager::getInstance().convertTextureCoord(source, size);
		return data;
	}

} // namespace MyGUI
