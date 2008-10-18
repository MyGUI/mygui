/*!
	@file
	@author		Albert Semenov
	@date		03/2008
	@module
*/
#include "MyGUI_RawRect.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_SkinManager.h"

namespace MyGUI
{

	struct RawRectStateData : public StateInfo
	{
		FloatRect rect;
	};

	const size_t COLOURRECT_COUNT_VERTEX = VERTEX_IN_QUAD;

	MYGUI_RTTI_CHILD_IMPLEMENT(RawRect, SubSkin);

	RawRect::RawRect(const SubWidgetInfo &_info, ICroppedRectangle * _parent) :
		SubSkin(_info, _parent),
		mRenderColourLT(0xFFFFFFFF),
		mRenderColourRT(0xFFFFFFFF),
		mRenderColourLB(0xFFFFFFFF),
		mRenderColourRB(0xFFFFFFFF),
		mColourLT(Ogre::ColourValue::White),
		mColourRT(Ogre::ColourValue::White),
		mColourLB(Ogre::ColourValue::White),
		mColourRB(Ogre::ColourValue::White),
		mRectTextureLT(FloatPoint(0, 0)),
		mRectTextureRT(FloatPoint(1, 0)),
		mRectTextureLB(FloatPoint(0, 1)),
		mRectTextureRB(FloatPoint(1, 1))
	{
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

		if (null != mRenderItem) mRenderItem->outOfDate();
	}

	void RawRect::setRectColour(const Ogre::ColourValue & _colourLT, const Ogre::ColourValue & _colourRT, const Ogre::ColourValue & _colourLB, const Ogre::ColourValue & _colourRB)
	{
		mColourLT = _colourLT;
		Ogre::Root::getSingleton().convertColourValue(mColourLT, &mRenderColourLT);
		mRenderColourLT = mCurrentAlpha | (mRenderColourLT & 0x00FFFFFF);

		mColourRT = _colourRT;
		Ogre::Root::getSingleton().convertColourValue(mColourRT, &mRenderColourRT);
		mRenderColourRT = mCurrentAlpha | (mRenderColourRT & 0x00FFFFFF);

		mColourLB = _colourLB;
		Ogre::Root::getSingleton().convertColourValue(mColourLB, &mRenderColourLB);
		mRenderColourLB = mCurrentAlpha | (mRenderColourLB & 0x00FFFFFF);

		mColourRB = _colourRB;
		Ogre::Root::getSingleton().convertColourValue(mColourRB, &mRenderColourRB);
		mRenderColourRB = mCurrentAlpha | (mRenderColourRB & 0x00FFFFFF);

		if (null != mRenderItem) mRenderItem->outOfDate();
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

	StateInfo * RawRect::createStateData(xml::xmlNodePtr _node, xml::xmlNodePtr _root)
	{
		const IntSize & size = SkinManager::getInstance().getTextureSize(_root->findAttribute("texture"));
		RawRectStateData * data = new RawRectStateData();
		const FloatRect & source = FloatRect::parse(_node->findAttribute("offset"));
		data->rect = SkinManager::getInstance().convertTextureCoord(source, size);
		return data;
	}

} // namespace MyGUI
