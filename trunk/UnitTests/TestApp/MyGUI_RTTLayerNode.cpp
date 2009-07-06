/*!
	@file
	@author		Albert Semenov
	@date		06/2008
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
#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_TextureManager.h"
#include "MyGUI_Gui.h"

namespace MyGUI
{

    /** Returns the closest power-of-two number greater or equal to value.
        @note 0 and 1 are powers of two, so 
            firstPO2From(0)==0 and firstPO2From(1)==1.
    */
    static uint32 firstPO2From(uint32 n)
    {
        --n;            
        n |= n >> 16;
        n |= n >> 8;
        n |= n >> 4;
        n |= n >> 2;
        n |= n >> 1;
        ++n;
        return n;
    }

	RTTLayerNode::RTTLayerNode(ILayer* _layer, ILayerNode* _parent) :
		LayerNode(_layer, _parent),
		mVertexBuffer(nullptr),
		mTexture(nullptr),
		mOutOfDate(false),
		mChacheUsing(true),
		mMajorUpdate(false),
		mIsAnimate(false),
		mDestroy(false)
	{
	}

	RTTLayerNode::~RTTLayerNode()
	{
		RenderManager& render = RenderManager::getInstance();

		if ( mVertexBuffer != nullptr )
		{
			render.destroyVertexBuffer( mVertexBuffer );
			mVertexBuffer = nullptr;
		}
		if ( mTexture != nullptr )
		{
			MyGUI::TextureManager::getInstance().destroyTexture( mTexture );
			//render.destroyTexture( mTexture );
			mTexture = nullptr;
		}
	}

	void RTTLayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{
		if (mMajorUpdate)
		{
			_update = true;
			mMajorUpdate = false;
		}

		if (!mChacheUsing)
		{
			LayerNode::renderToTarget(_target, _update);
			return;
		}

		RenderManager& render = RenderManager::getInstance();
		if (mVertexBuffer == nullptr)
		{
			mVertexBuffer = render.createVertexBuffer();
			mVertexBuffer->setVertextCount(VertexQuad::VertexCount);
			mData.resize(1);

			_update = true;
			mOutOfDate = true;
		}

		if (mOutOfDate && !mLayerItems.empty())
		{
			const IntCoord& coord = mLayerItems.front()->getLayerItemCoord();
			if (coord != mCurrentCoord)
			{
				mCurrentCoord = coord;
				checkTexture();
				_update = true;
				if (mTexture != nullptr)
				{
					mTexture->getInfo().setOffset(mCurrentCoord.left, mCurrentCoord.top);
				}
			}
		}

		if (mTexture == nullptr) return;

		if (_update)
		{
			const RenderTargetInfo& info = _target->getInfo();

			float vertex_z = info.maximumDepth;

			float vertex_left = ((info.pixScaleX * (float)(mCurrentCoord.left) + info.hOffset) * 2) - 1;
			float vertex_right = vertex_left + (info.pixScaleX * (float)mCurrentCoord.width * 2);
			float vertex_top = -(((info.pixScaleY * (float)(mCurrentCoord.top) + info.vOffset) * 2) - 1);
			float vertex_bottom = vertex_top - (info.pixScaleY * (float)mCurrentCoord.height * 2);

			float texture_u = (float)mCurrentCoord.width / (float)mTexture->getWidth();
			float texture_v = (float)mCurrentCoord.height / (float)mTexture->getHeight();
			float texture_v2 = 0;
			if (info.rttFlipY)
			{
				texture_v = 1 - texture_v;
				texture_v2 = 1;
			}

			mDefaultData.set(
				vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z,
				0, texture_v2, texture_u, texture_v, 0xFFFFFFFF
				);
		}

		// анимируем и проверяем, использовалась ли анимация
		bool need_update = mIsAnimate;
		mIsAnimate = false;
		size_t count_quad = 1;
		mData.resize(count_quad);
		mData[0] = mDefaultData;

		Enumerator<VectorLayerNodeAnimation> anim = Enumerator<VectorLayerNodeAnimation>(mLayerNodeAnimation);
		while (anim.next())
		{
			float time = Gui::getInstance().getLastFrameTime();
			count_quad = anim->animate(_update, count_quad, mData, time, mVertexBuffer, mTexture, _target->getInfo(), mCurrentCoord, mIsAnimate);
		}

		if (mIsAnimate)
		{
			// блитим анимацию
			mVertexBuffer->setVertextCount(count_quad * VertexQuad::VertexCount);
			VertexQuad* quad = (VertexQuad*)mVertexBuffer->lock();

			for (size_t index=0; index<count_quad; ++index)
			{
				// копируем дефолтные данные
				quad[index].vertex[VertexQuad::CornerLT] = mData[index].vertex[QuadData::CornerLT];
				quad[index].vertex[VertexQuad::CornerRT] = mData[index].vertex[QuadData::CornerRT];
				quad[index].vertex[VertexQuad::CornerLB] = mData[index].vertex[QuadData::CornerLB];
				quad[index].vertex[VertexQuad::CornerLB2] = mData[index].vertex[QuadData::CornerLB];
				quad[index].vertex[VertexQuad::CornerRT2] = mData[index].vertex[QuadData::CornerRT];
				quad[index].vertex[VertexQuad::CornerRB] = mData[index].vertex[QuadData::CornerRB];
			}

			mVertexBuffer->unlock();
		}
		else
		{
			count_quad = 1;

			if (_update || need_update)
			{
				mVertexBuffer->setVertextCount(count_quad * VertexQuad::VertexCount);
				VertexQuad* quad = (VertexQuad*)mVertexBuffer->lock();

				// копируем дефолтные данные
				quad->vertex[VertexQuad::CornerLT] = mDefaultData.vertex[QuadData::CornerLT];
				quad->vertex[VertexQuad::CornerRT] = mDefaultData.vertex[QuadData::CornerRT];
				quad->vertex[VertexQuad::CornerLB] = mDefaultData.vertex[QuadData::CornerLB];
				quad->vertex[VertexQuad::CornerLB2] = mDefaultData.vertex[QuadData::CornerLB];
				quad->vertex[VertexQuad::CornerRT2] = mDefaultData.vertex[QuadData::CornerRT];
				quad->vertex[VertexQuad::CornerRB] = mDefaultData.vertex[QuadData::CornerRB];

				mVertexBuffer->unlock();
			}
		}

		if (_update || mOutOfDate)
		{

			if (!mLayerItems.empty())
			{
				mTexture->begin();
				LayerNode::renderToTarget(mTexture, _update);
				mTexture->end();
			}

			mOutOfDate = false;
		}

		if (!(!mIsAnimate && mDestroy))
		{
			_target->doRender(mVertexBuffer, mTexture, count_quad * VertexQuad::VertexCount);
		}
	}

	void RTTLayerNode::checkTexture()
	{
		if (mTextureSize.width < mCurrentCoord.width || mTextureSize.height < mCurrentCoord.height)
		{
			RenderManager& render = RenderManager::getInstance();

			if (mTexture != nullptr)
			{
				MyGUI::TextureManager::getInstance().destroyTexture( mTexture );
				//render.destroyTexture(mTexture);
				mTexture = nullptr;
			}

			if (mCurrentCoord.width > 0 && mCurrentCoord.height > 0)
			{
				mTextureSize.set(firstPO2From(mCurrentCoord.width), firstPO2From(mCurrentCoord.height));
				mTexture = MyGUI::TextureManager::getInstance().createTexture(utility::toString((size_t)this, "_texture_node"));
				//mTexture = render.createTexture(utility::toString((size_t)this, "_texture_node"), "General");
				mTexture->createManual(mTextureSize.width, mTextureSize.height, TextureUsage::RenderTarget, PixelFormat::A8R8G8B8);
			}
		}
	}

	void RTTLayerNode::outOfDate(RenderItem* _item)
	{
		_item->outOfDate();
		mOutOfDate = true;
	}

	void RTTLayerNode::setCacheUsing(bool _value)
	{
		mChacheUsing = _value;
		mMajorUpdate = true;
	}

	void RTTLayerNode::attachLayerItem(ILayerItem* _item)
	{
		Enumerator<VectorLayerNodeAnimation> anim = Enumerator<VectorLayerNodeAnimation>(mLayerNodeAnimation);
		while (anim.next())
			anim->create();
		
		Base::attachLayerItem(_item);
	}

	void RTTLayerNode::detachLayerItem(ILayerItem* _item)
	{
		Enumerator<VectorLayerNodeAnimation> anim = Enumerator<VectorLayerNodeAnimation>(mLayerNodeAnimation);
		while (anim.next())
			anim->destroy();

		Base::detachLayerItem(_item);
	}

	void RTTLayerNode::addLayerNodeAnimation(LayerNodeAnimation* _impl)
	{
		mLayerNodeAnimation.push_back(_impl);
		mMajorUpdate = true;
	}

} // namespace MyGUI
