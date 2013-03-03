/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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

	RenderItem::RenderItem() :
		mTexture(nullptr),
		mNeedVertexCount(0),
		mOutOfDate(false),
		mCountVertex(0),
		mCurrentUpdate(true),
		mCurrentVertex(nullptr),
		mLastVertexCount(0),
		mVertexBuffer(nullptr),
		mRenderTarget(nullptr),
		mCompression(false),
		mManualRender(false)
	{
		mVertexBuffer = RenderManager::getInstance().createVertexBuffer();
	}

	RenderItem::~RenderItem()
	{
		RenderManager::getInstance().destroyVertexBuffer(mVertexBuffer);
		mVertexBuffer = nullptr;
	}

	void RenderItem::renderToTarget(IRenderTarget* _target, bool _update)
	{
		if (mTexture == nullptr)
			return;

		mRenderTarget = _target;

		mCurrentUpdate = _update;

		if (mOutOfDate || _update)
		{
			mCountVertex = 0;
			Vertex* buffer = mVertexBuffer->lock();
			if (buffer != nullptr)
			{
				for (VectorDrawItem::iterator iter = mDrawItems.begin(); iter != mDrawItems.end(); ++iter)
				{
					// перед вызовом запоминаем позицию в буфере
					mCurrentVertex = buffer;
					mLastVertexCount = 0;

					(*iter).first->doRender();

					// колличество отрисованных вершин
					MYGUI_DEBUG_ASSERT(mLastVertexCount <= (*iter).second, "It is too much vertexes");
					buffer += mLastVertexCount;
					mCountVertex += mLastVertexCount;
				}

				mVertexBuffer->unlock();
			}

			mOutOfDate = false;
		}

		// хоть с 0 не выводиться батч, но все равно не будем дергать стейт и операцию
		if (0 != mCountVertex)
		{
#if MYGUI_DEBUG_MODE == 1
			if (!RenderManager::getInstance().checkTexture(mTexture))
			{
				mTexture = nullptr;
				MYGUI_EXCEPT("texture pointer is not valid, texture name '" << mTextureName << "'");
				return;
			}
#endif
			// непосредственный рендринг
			if (mManualRender)
			{
				for (VectorDrawItem::iterator iter = mDrawItems.begin(); iter != mDrawItems.end(); ++iter)
					(*iter).first->doManualRender(mVertexBuffer, mTexture, mCountVertex);
			}
			else
			{
				_target->doRender(mVertexBuffer, mTexture, mCountVertex);
			}
		}
	}

	void RenderItem::removeDrawItem(ISubWidget* _item)
	{
		for (VectorDrawItem::iterator iter = mDrawItems.begin(); iter != mDrawItems.end(); ++iter)
		{
			if ((*iter).first == _item)
			{
				mNeedVertexCount -= (*iter).second;
				mDrawItems.erase(iter);
				mOutOfDate = true;

				mVertexBuffer->setVertexCount(mNeedVertexCount);

				// если все отдетачились, расскажем отцу
				if (mDrawItems.empty())
				{
					mTexture = nullptr;
					mCompression = true;
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
		for (VectorDrawItem::iterator iter = mDrawItems.begin(); iter != mDrawItems.end(); ++iter)
		{
			MYGUI_ASSERT((*iter).first != _item, "DrawItem exist");
		}
#endif

		mDrawItems.push_back(DrawItemInfo(_item, _count));
		mNeedVertexCount += _count;
		mOutOfDate = true;

		mVertexBuffer->setVertexCount(mNeedVertexCount);
	}

	void RenderItem::reallockDrawItem(ISubWidget* _item, size_t _count)
	{
		for (VectorDrawItem::iterator iter = mDrawItems.begin(); iter != mDrawItems.end(); ++iter)
		{
			if ((*iter).first == _item)
			{
				// если нужно меньше, то ниче не делаем
				if ((*iter).second < _count)
				{
					mNeedVertexCount -= (*iter).second;
					mNeedVertexCount += _count;
					(*iter).second = _count;
					mOutOfDate = true;

					mVertexBuffer->setVertexCount(mNeedVertexCount);
				}
				return;
			}
		}
		MYGUI_EXCEPT("DrawItem not found");
	}

	void RenderItem::setTexture(ITexture* _value)
	{
		if (mTexture == _value)
			return;

		//MYGUI_DEBUG_ASSERT(mVertexBuffer->getVertexCount() == 0, "change texture only empty buffer");
		MYGUI_DEBUG_ASSERT(mNeedVertexCount == 0, "change texture only empty buffer");

		mTexture = _value;

#if MYGUI_DEBUG_MODE == 1
		mTextureName = mTexture == nullptr ? "" : mTexture->getName();
#endif
	}

	ITexture* RenderItem::getTexture()
	{
		return mTexture;
	}

	bool RenderItem::getCompression()
	{
		bool result = mCompression;
		mCompression = false;
		return result;
	}

	void RenderItem::setManualRender(bool _value)
	{
		mManualRender = _value;
	}

	bool RenderItem::getManualRender() const
	{
		return mManualRender;
	}

	void RenderItem::outOfDate()
	{
		mOutOfDate = true;
	}

	bool RenderItem::isOutOfDate() const
	{
		return mOutOfDate;
	}

	size_t RenderItem::getNeedVertexCount() const
	{
		return mNeedVertexCount;
	}

	size_t RenderItem::getVertexCount() const
	{
		return mCountVertex;
	}

	bool RenderItem::getCurrentUpdate() const
	{
		return mCurrentUpdate;
	}

	Vertex* RenderItem::getCurrentVertexBuffer() const
	{
		return mCurrentVertex;
	}

	void RenderItem::setLastVertexCount(size_t _count)
	{
		mLastVertexCount = _count;
	}

	IRenderTarget* RenderItem::getRenderTarget()
	{
		return mRenderTarget;
	}

} // namespace MyGUI
