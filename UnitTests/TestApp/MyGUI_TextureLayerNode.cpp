/*!
	@file
	@author		Albert Semenov
	@date		02/2008
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
#include "MyGUI_TextureLayerNode.h"
#include "MyGUI_ILayerItem.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_RenderManager.h"
#include <Ogre.h>

namespace MyGUI
{

    /** Returns the closest power-of-two number greater or equal to value.
        @note 0 and 1 are powers of two, so 
            firstPO2From(0)==0 and firstPO2From(1)==1.
    */
    static FORCEINLINE uint32 firstPO2From(uint32 n)
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

	const float VERTEXT_IN_QUAD = 6;

	bool TextureLayerNode::msUseCashe = true;
	bool TextureLayerNode::msUpdate = true;

	TextureLayerNode::TextureLayerNode(ILayer* _layer, TextureLayerNode * _parent) :
		mCountUsing(0),
		mParent(_parent),
		mLayer(_layer),
		mLayerItem(nullptr),
		mVertexBuffer(nullptr),
		mTexture(nullptr),
		mOutOfDate(false)
	{
		RenderManager& render = RenderManager::getInstance();

		mVertexBuffer = render.createVertexBuffer();
		mVertexBuffer->setVertextCount(VERTEXT_IN_QUAD);

		mCurrentCoord.set(0, 0, 0, 0);
		checkTexture();
	}

	TextureLayerNode::~TextureLayerNode()
	{
		RenderManager& render = RenderManager::getInstance();

		delete mVertexBuffer;
		render.destroyTexture(mTexture);
		/*for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			delete (*iter);
		}
		mSecondRenderItems.clear();*/

		// удаляем дочерние узлы
		/*for (VectorTextureLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			delete (*iter);
		}
		mChildItems.clear();*/
	}

	ILayerNode* TextureLayerNode::createItemNode()
	{
		return nullptr;
	}

	void TextureLayerNode::destroyItemNode()
	{
		mLayer->destroyItemNode(this);
	}

	void TextureLayerNode::destroyItemNode(TextureLayerNode* _item)
	{
	}

	TextureLayerNode * TextureLayerNode::upItemNode(TextureLayerNode* _item)
	{
		return this;
	}

	void TextureLayerNode::upItemNode()
	{
		mLayer->upItemNode(this);
	}

	void TextureLayerNode::renderToTarget(IRenderTarget* _target, bool _update)
	{

		if (msUpdate)
		{
			_update = true;
			msUpdate = false;
		}

		if (!msUseCashe)
		{
			for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
			{
				(*iter)->renderToTarget(_target, _update);
			}
			for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
			{
				(*iter)->renderToTarget(_target, _update);
			}

			return;
		}

		RenderManager& render = RenderManager::getInstance();

		if (_update)
		{
			//IntCoord mCurrentCoord(0, 0, mTextureSize.width, mTextureSize.height);
			//IntPoint absolute_point(0, 0);

			if (mLayerItem != nullptr)
			{
				Widget* widget = dynamic_cast<Widget*>(mLayerItem);
				if (widget != nullptr)
				{
					mAbsolutePoint = widget->getAbsolutePosition();
					mCurrentCoord = widget->getCoord();

					checkTexture();
				}
			}

			VertexQuad* quad = (VertexQuad*)mVertexBuffer->lock();

			const RenderTargetInfo& info = _target->getInfo();

			float vertex_z = info.maximumDepth;

			float vertex_left = ((info.pixScaleX * (float)(/*mCurrentCoord.left + */mAbsolutePoint.left) + info.hOffset) * 2) - 1;
			float vertex_right = vertex_left + (info.pixScaleX * (float)mTextureSize.width * 2);
			float vertex_top = -(((info.pixScaleY * (float)(/*mCurrentCoord.top + */mAbsolutePoint.top) + info.vOffset) * 2) - 1);
			float vertex_bottom = vertex_top - (info.pixScaleY * (float)mTextureSize.height * 2);

			quad->set(
				vertex_left, vertex_top, vertex_right, vertex_bottom, vertex_z,
				0, 0, 1, 1, 0xFFFFFFFF
				);

			mVertexBuffer->unlock();
		}

		if (_update || mOutOfDate)
		{
			mTexture->begin();

			// сначала отрисовываем свое
			for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
			{
				(*iter)->renderToTarget(mTexture, _update);
			}
			for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
			{
				(*iter)->renderToTarget(mTexture, _update);
			}

			mTexture->end();

			mOutOfDate = false;
		}

		_target->doRender(mVertexBuffer, mTexture, VERTEXT_IN_QUAD);

		// теперь отрисовываем дочерние узлы
		/*for (VectorTextureLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			(*iter)->_render(_update);
		}*/

	}

	ILayerItem * TextureLayerNode::getLayerItemByPoint(int _left, int _top)
	{
		// сначала пикаем детей
		/*for (VectorTextureLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
		}*/

		// а теперь себя
		if (mLayerItem != nullptr)
		{
			ILayerItem * item = mLayerItem->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
		}
		/*for (VectorLayerItem::iterator iter=mLayerItems.begin(); iter!=mLayerItems.end(); ++iter)
		{
			ILayerItem * item = (*iter)->getLayerItemByPoint(_left, _top);
			if (nullptr != item) return item;
		}*/

		return nullptr;
	}

	/*void TextureLayerNode::_update()
	{
		// буферы освобождаются по одному всегда

		if (mFirstRenderItems.size() > 1)
		{
			// пытаемся поднять пустой буфер выше полных
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end())
			{
				if ((*iter1)->getNeedVertexCount() == 0)
				{
					RenderItem * tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
		}
	}*/

	RenderItem* TextureLayerNode::addToRenderItem(const std::string& _texture, IDrawItem* _item)
	{
		return addToRenderItem(_texture, !_item->isType<ISubWidgetText>());
	}

	RenderItem* TextureLayerNode::addToRenderItem(const std::string& _texture, bool _first)
	{
		// для первичной очереди нужен порядок
		if (_first)
		{
			if (mFirstRenderItems.empty())
			{

				// создаем новый буфер
				RenderItem * item = new RenderItem(_texture/*, this*/);
				mFirstRenderItems.push_back(item);

				return item;
			}

			// если последний буфер пустой, то мона не создавать
			if (mFirstRenderItems.back()->getNeedVertexCount() == 0)
			{
				// пустых может быть сколько угодно, нужен самый первый из пустых
				for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter)
				{
					if ((*iter)->getNeedVertexCount() == 0)
					{
						// а теперь внимание, если перед пустым наш, то его и юзаем
						if (iter != mFirstRenderItems.begin())
						{
							VectorRenderItem::iterator prev = iter - 1;
							if ((*prev)->getTextureName() == _texture)
							{
								return (*prev);
							}
						}
						(*iter)->setTextureName(_texture);
						return (*iter);
					}
				}
			}

			// та же текстура
			if (mFirstRenderItems.back()->getTextureName() == _texture)
			{
				return mFirstRenderItems.back();
			}

			// создаем новый буфер
			RenderItem * item = new RenderItem(_texture/*, this*/);
			mFirstRenderItems.push_back(item);

			return item;
		}

		// для второй очереди порядок неважен
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			// либо такая же текстура, либо пустой буфер
			if ((*iter)->getTextureName() == _texture)
			{
				return (*iter);
			}
			else if ((*iter)->getNeedVertexCount() == 0)
			{
				(*iter)->setTextureName(_texture);
				return (*iter);
			}

		}
		// не найденно создадим новый
		mSecondRenderItems.push_back(new RenderItem(_texture/*, this*/));
		return mSecondRenderItems.back();
	}

	size_t TextureLayerNode::getItemCount()
	{
		size_t count = 1;
		// сначала пикаем детей
		/*for (VectorTextureLayerItemKeeper::iterator iter = mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			count += (*iter)->getItemCount();
		}*/
		return count;
	}

	bool TextureLayerNode::existItemNode(TextureLayerNode* _item)
	{
		/*for (VectorTextureLayerItemKeeper::iterator iter=mChildItems.begin(); iter!=mChildItems.end(); ++iter)
		{
			if ((*iter) == _item || (*iter)->existItemNode(_item)) return true;
		}*/
		return false;
	}

	void TextureLayerNode::attachLayerItem(ILayerItem* _root)
	{
		mLayerItem = static_cast<LayerItem*>(_root);
		//mLayerItems.push_back(_root);
		_root->attachItemToNode(mLayer, this);
	}

	void TextureLayerNode::detachLayerItem(ILayerItem* _root)
	{
		mLayerItem = nullptr;
		/*for (VectorLayerItem::iterator iter=mLayerItems.begin(); iter!=mLayerItems.end(); ++iter)
		{
			if ((*iter) == _root)
			{
				(*iter) = mLayerItems.back();
				mLayerItems.pop_back();
				return;
			}
		}
		MYGUI_EXCEPT("layer item not found");*/
	}

	ILayer* TextureLayerNode::getLayer()
	{
		return mLayer;
	}

	void TextureLayerNode::checkTexture()
	{
		if (mTextureSize.width < mCurrentCoord.width || mTextureSize.height < mCurrentCoord.height)
		{
			mTextureSize.set(firstPO2From(mCurrentCoord.width), firstPO2From(mCurrentCoord.height));

			RenderManager& render = RenderManager::getInstance();

			if (mTexture != nullptr)
			{
				render.destroyTexture(mTexture);
				mTexture = nullptr;
			}

			mTexture = render.createTexture(generateTextureName(), "General");
			mTexture->createManual(mTextureSize.width, mTextureSize.height, TextureUsage::RenderTarget, PixelFormat::A8R8G8B8);
		}
	}

	std::string TextureLayerNode::generateTextureName()
	{
		return utility::toString((size_t)this, "_texture_node");
	}

	void TextureLayerNode::outOfDate(RenderItem* _item)
	{
		_item->outOfDate();
		mOutOfDate = true;
	}

} // namespace MyGUI
