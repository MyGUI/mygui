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

namespace MyGUI
{

	float TextureLayerNode::mMaximumDepth = 0;
	float TextureLayerNode::mPixScaleX = 1;
	float TextureLayerNode::mPixScaleY = 1;
    float TextureLayerNode::mHOffset = 0;
	float TextureLayerNode::mVOffset = 0;
	float TextureLayerNode::mAspectCoef = 1;

	const float VERTEXT_IN_QUAD = 6;

	TextureLayerNode::TextureLayerNode(ILayer* _layer, TextureLayerNode * _parent) :
		mCountUsing(0),
		mParent(_parent),
		mLayer(_layer)
	{
		RenderManager& render = RenderManager::getInstance();

		mVertexBuffer = render.createVertexBuffer();
		mVertexBuffer->setVertextCount(VERTEXT_IN_QUAD);

		mTexture = render.createTexture(utility::toString((size_t)this, "_texture_node"), "General");
		//mTexture->setManualResourceLoader(this);
		mTexture->createManual(1024, 1024, TextureUsage::DynamicWriteOnlyDiscardable, PixelFormat::A8R8G8B8);

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

	void TextureLayerNode::_render(bool _update)
	{

		RenderManager& render = RenderManager::getInstance();

		if (_update)
		{
			mViewSize = render.getViewSize();
			mMaximumDepth = render.getMaximumDepth();

			// новый размер
			mPixScaleX = 1.0 / float(mViewSize.width);
			mPixScaleY = 1.0 / float(mViewSize.height);
			mAspectCoef = float(mViewSize.height) / float(mViewSize.width);

			const FloatSize& size_offset = render.getTexelOffset();

			mHOffset = size_offset.width / mViewSize.width;
			mVOffset = size_offset.height / mViewSize.height;

			IntCoord mCurrentCoord(0, 0, 128, 128);
			IntPoint absolute_point(0, 0);

			if (mLayerItem != nullptr)
			{
				Widget* widget = dynamic_cast<Widget*>(mLayerItem);
				if (widget != nullptr)
				{
					absolute_point = widget->getAbsolutePosition();
					mCurrentCoord = widget->getCoord();
				}
			}

			Vertex* vertex = (Vertex*)mVertexBuffer->lock();

			float vertex_z = mMaximumDepth;

			float vertex_left = ((mPixScaleX * (float)(/*mCurrentCoord.left + */absolute_point.left) + mHOffset) * 2) - 1;
			float vertex_right = vertex_left + (mPixScaleX * (float)mCurrentCoord.width * 2);
			float vertex_top = -(((mPixScaleY * (float)(/*mCurrentCoord.top + */absolute_point.top) + mVOffset) * 2) - 1);
			float vertex_bottom = vertex_top - (mPixScaleY * (float)mCurrentCoord.height * 2);

			vertex[Vertex::CornerLT].set(vertex_left, vertex_top, vertex_z, 0, 0, 0xFFFFFFFF);
			vertex[Vertex::CornerRT].set(vertex_right, vertex_top, vertex_z, 1, 0, 0xFFFFFFFF);
			vertex[Vertex::CornerLB].set(vertex_left, vertex_bottom, vertex_z, 0, 1, 0xFFFFFFFF);
			vertex[Vertex::CornerRB].set(vertex_right, vertex_bottom, vertex_z, 1, 1, 0xFFFFFFFF);
			vertex[Vertex::CornerRT2] = vertex[Vertex::CornerRT];
			vertex[Vertex::CornerLB2] = vertex[Vertex::CornerLB];

			mVertexBuffer->unlock();
		}


		// сначала отрисовываем свое
		for (VectorIDrawItem::iterator iter=mRenderItems.begin(); iter!=mRenderItems.end(); ++iter)
		{
			(*iter)->doRender();
		}

		render.doRender(mVertexBuffer, mTexture, VERTEXT_IN_QUAD);


		// сначала отрисовываем свое
		/*for (VectorIDrawItem::iterator iter=mRenderItems.begin(); iter!=mRenderItems.end(); ++iter)
		{
			(*iter)->_render(_update);
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter)
		{
			(*iter)->_render(_update);
		}*/

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

	void TextureLayerNode::_update()
	{
		// буферы освобождаются по одному всегда

		/*if (mFirstRenderItems.size() > 1)
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
		}*/
	}

	IRenderItem * TextureLayerNode::addToRenderItem(const std::string& _texture, IDrawItem* _item)
	{
		mRenderItems.push_back(_item);
		return nullptr;
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

} // namespace MyGUI
