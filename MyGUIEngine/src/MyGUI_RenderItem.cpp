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
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerNode.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_RenderManager.h"
#include "MyGUI_DataManager.h"
#include "MyGUI_RenderManager.h"

namespace MyGUI
{

	//float RenderItem::mMaximumDepth = 0;
	//float RenderItem::mPixScaleX = 1;
	//float RenderItem::mPixScaleY = 1;
    //float RenderItem::mHOffset = 0;
	//float RenderItem::mVOffset = 0;
	//float RenderItem::mAspectCoef = 1;

	RenderItem::RenderItem(const std::string& _texture/*, LayerNode * _parent*/) :
		mTextureName(_texture),
		mNeedVertexCount(0),
		mOutDate(false),
		//mParent(_parent),
		mCountVertex(0),
		mCurrentUpdate(true),
		mCurrentVertext(nullptr),
		mLastVertextCount(0),
		mVertexBuffer(nullptr),
		mRenderTarget(nullptr)
	{
		//mLayerManager = LayerManager::getInstancePtr();

		mVertexBuffer = RenderManager::getInstance().createVertexBuffer();

		//mMaximumDepth = RenderManager::getInstance().getMaximumDepth();
	}

	RenderItem::~RenderItem()
	{
		RenderManager::getInstance().destroyVertexBuffer(mVertexBuffer);
		mVertexBuffer = nullptr;
	}

	void RenderItem::renderToTarget(IRenderTarget* _target, bool _update)
	{
		if (mTextureName.empty()) return;

		mRenderTarget = _target;

		RenderManager& render = RenderManager::getInstance();

		//if (_update)
		//{
			//mViewSize = render.getViewSize();
			//mMaximumDepth = render.getMaximumDepth();

			// новый размер
			//mPixScaleX = 1.0 / float(mViewSize.width);
			//mPixScaleY = 1.0 / float(mViewSize.height);
			//mAspectCoef = float(mViewSize.height) / float(mViewSize.width);

			//const FloatSize& size_offset = render.getTexelOffset();

	        //mHOffset = size_offset.width / mViewSize.width;
		    //mVOffset = size_offset.height / mViewSize.height;
		//}

		mCurrentUpdate = _update;

		if (mOutDate || _update)
		{
			mCountVertex = 0;
			Vertex * buffer = (Vertex*)mVertexBuffer->lock();

			for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter)
			{
				// перед вызовом запоминаем позицию в буфере
				mCurrentVertext = buffer;
				mLastVertextCount = 0;

				(*iter).first->doRender();

				// колличество отрисованных вершин
				MYGUI_DEBUG_ASSERT(mLastVertextCount <= (*iter).second, "It is too much vertexes");
				buffer += mLastVertextCount;
				mCountVertex += mLastVertextCount;
			}

			mVertexBuffer->unlock();

			mOutDate = false;
		}

		// хоть с 0 не выводиться батч, но все равно не будем дергать стейт и операцию
		if (0 != mCountVertex)
		{
			// FIXME текстуру сразу брать
			ITexture* texture = RenderManager::getInstance().getTexture(mTextureName);

			// непосредственный рендринг
			_target->doRender(mVertexBuffer, texture, mCountVertex);
		}
	}

	void RenderItem::removeDrawItem(ISubWidget* _item)
	{
		for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter)
		{
			if ((*iter).first == _item)
			{
				mNeedVertexCount -= (*iter).second;
				mDrawItems.erase(iter);
				mOutDate = true;

				mVertexBuffer->setVertextCount(mNeedVertexCount);

				// если все отдетачились, расскажем отцу
				if (mDrawItems.empty())
				{
					mTextureName.clear();
					//mParent->_update();
				}

				return;
			}
		}
		MYGUI_EXCEPT("DrawItem not found");
	}

	void RenderItem::addDrawItem(ISubWidget* _item, size_t _count)
	{

// проверяем только в дебаге
#if MYGUI_DEBUG_MODE == 1
		for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter)
		{
			MYGUI_ASSERT((*iter).first != _item, "DrawItem exist");
		}
#endif

		mDrawItems.push_back(DrawItemInfo(_item, _count));
		mNeedVertexCount += _count;
		mOutDate = true;

		mVertexBuffer->setVertextCount(mNeedVertexCount);
	}

	void RenderItem::reallockDrawItem(ISubWidget* _item, size_t _count)
	{
		for (VectorDrawItem::iterator iter=mDrawItems.begin(); iter!=mDrawItems.end(); ++iter)
		{
			if ((*iter).first == _item)
			{
				// если нужно меньше, то ниче не делаем
				if ((*iter).second < _count)
				{
					mNeedVertexCount -= (*iter).second;
					mNeedVertexCount += _count;
					(*iter).second = _count;
					mOutDate = true;

					mVertexBuffer->setVertextCount(mNeedVertexCount);
				}
				return;
			}
		}
		MYGUI_EXCEPT("DrawItem not found");
	}

	void RenderItem::setTextureName(const std::string& _texture)
	{
		MYGUI_DEBUG_ASSERT(mVertexBuffer->getVertextCount() == 0, "change texture only empty buffer");
		MYGUI_DEBUG_ASSERT(mNeedVertexCount == 0, "change texture only empty buffer");
		mTextureName = _texture;
	}

} // namespace MyGUI
