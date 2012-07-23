/*!
	@file
	@author		Albert Semenov
	@date		06/2008
*/

#include "MyGUI_RTTLayerNode.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Bitwise.h"

namespace MyGUI
{

	RTTLayerNode::RTTLayerNode(ILayer* _layer, ILayerNode* _parent) :
		LayerNode(_layer, _parent),
		mVertexBuffer(nullptr),
		mTexture(nullptr),
		mOutOfDate(false),
		mMajorUpdate(false),
		mChacheUsing(true),
		mDestroy(false),
		mIsAnimate(false)
	{
		mTimer.reset();
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
			MyGUI::RenderManager::getInstance().destroyTexture( mTexture );
			mTexture = nullptr;
		}

		for (VectorLayerNodeAnimation::iterator item = mLayerNodeAnimation.begin(); item != mLayerNodeAnimation.end(); ++item)
			delete (*item);
		mLayerNodeAnimation.clear();
	}

	void RTTLayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{
		unsigned long time = mTimer.getMilliseconds();
		mTimer.reset();

		float frameTime = (float)((double)(time) / (double)1000);
		// скорее всего рендер таргет пересоздается,
		// необходимо пересчитать смещение рендер таргета
		if (_update)
		{
			mOutOfDate = true;
			mCurrentCoord.clear();
		}

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
			mVertexBuffer->setVertexCount(VertexQuad::VertexCount);
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
					MyGUI::IRenderTarget* target = mTexture->getRenderTarget();
					if (target != nullptr)
					{
						// смещение рендер таргета
						target->getInfo().setOffset(mCurrentCoord.left, mCurrentCoord.top);
					}
				}
			}
		}

		if (mTexture == nullptr)
			return;

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

			mDefaultData.set(
				vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z,
				0, 0, texture_u, texture_v, 0xFFFFFFFF
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
			count_quad = anim->animate(_update, count_quad, mData, frameTime, mVertexBuffer, mTexture, _target->getInfo(), mCurrentCoord, mIsAnimate);
		}

		if (mIsAnimate)
		{
			// блитим анимацию
			mVertexBuffer->setVertexCount(count_quad * VertexQuad::VertexCount);
			VertexQuad* quad = reinterpret_cast<VertexQuad*>(mVertexBuffer->lock());

			for (size_t index = 0; index < count_quad; ++index)
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
				mVertexBuffer->setVertexCount(count_quad * VertexQuad::VertexCount);
				VertexQuad* quad = reinterpret_cast<VertexQuad*>(mVertexBuffer->lock());

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
				MyGUI::IRenderTarget* target = mTexture->getRenderTarget();
				if (target != nullptr)
				{
					target->begin();
					LayerNode::renderToTarget(target, _update);
					target->end();
				}
			}

			mOutOfDate = false;
		}

		if (mIsAnimate || !mDestroy)
		{
			_target->doRender(mVertexBuffer, mTexture, count_quad * VertexQuad::VertexCount);
		}
	}

	void RTTLayerNode::checkTexture()
	{
		if (mTextureSize.width < mCurrentCoord.width || mTextureSize.height < mCurrentCoord.height)
		{
			//RenderManager& render = RenderManager::getInstance();

			if (mTexture != nullptr)
			{
				MyGUI::RenderManager::getInstance().destroyTexture( mTexture );
				mTexture = nullptr;
			}

			if (mCurrentCoord.width > 0 && mCurrentCoord.height > 0)
			{
				mTextureSize.set(MyGUI::Bitwise::firstPO2From(mCurrentCoord.width), MyGUI::Bitwise::firstPO2From(mCurrentCoord.height));
				mTexture = MyGUI::RenderManager::getInstance().createTexture(utility::toString((size_t)this, "_texture_node"));
				mTexture->createManual(mTextureSize.width, mTextureSize.height, TextureUsage::RenderTarget, PixelFormat::R8G8B8A8);
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
