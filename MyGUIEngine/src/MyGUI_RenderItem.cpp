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

namespace MyGUI
{

	RenderItem::RenderItem()
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
				for (auto& item : mDrawItems)
				{
					mCurrentVertex = buffer;
					mLastVertexCount = 0;

					item.first->doRender();

					MYGUI_DEBUG_ASSERT(mLastVertexCount <= item.second, "It is too much vertexes");
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
				auto textureName = mTexture ? mTexture->getName() : std::string();
				mTexture = nullptr;
				MYGUI_EXCEPT("texture pointer is not valid, texture name '" << textureName << "'");
				return;
			}
#endif
			// непосредственный рендринг
			if (mManualRender)
			{
				for (auto& item : mDrawItems)
					item.first->doManualRender(mVertexBuffer, mTexture, mCountVertex);
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
					mNeedCompression = true;
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
		for (const auto& item : mDrawItems)
		{
			MYGUI_ASSERT(item.first != _item, "DrawItem exist");
		}
#endif

		mDrawItems.emplace_back(_item, _count);
		mNeedVertexCount += _count;
		mOutOfDate = true;

		mVertexBuffer->setVertexCount(mNeedVertexCount);
	}

	void RenderItem::reallockDrawItem(ISubWidget* _item, size_t _count)
	{
		for (auto& item : mDrawItems)
		{
			if (item.first == _item)
			{
				// если нужно меньше, то ниче не делаем
				if (item.second < _count)
				{
					mNeedVertexCount -= item.second;
					mNeedVertexCount += _count;
					item.second = _count;
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

		MYGUI_DEBUG_ASSERT(mVertexBuffer->getVertexCount() == 0, "change texture only empty buffer");
		MYGUI_DEBUG_ASSERT(mNeedVertexCount == 0, "change texture only empty buffer");

		mTexture = _value;
	}

	ITexture* RenderItem::getTexture() const
	{
		return mTexture;
	}

	void RenderItem::setNeedCompression(bool _compression)
	{
		mNeedCompression = _compression;
	}

	bool RenderItem::getNeedCompression() const
	{
		return mNeedCompression;
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
