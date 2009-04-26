/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#include "RenderItem.h"

namespace MyGUI
{

	RenderItem::RenderItem(const std::string& _texture) :
		mTexture(_texture),
		mData(0),
		mCount(0),
		mUpdate(true),
		mCurrentData(0)
	{
		mData = new int[mCount];
	}

	void RenderItem::addDrawItem(IDrawItem* _item, size_t _count)
	{
		// удаляем старые данные
		delete mData; 
		mData = 0;

		// обновляем информацию
		mDataInfo.push_back(PairDrawData(_item, _count));

		mCount += _count;

		// создаем новые данные
		mData = new int[mCount];

		for (size_t index=0; index<mCount; ++index)
		{
			mData[index] = (int)index + 1;
		}

		// все данные нужно обновить
		mUpdate = true;
	}

	void RenderItem::doRender(bool _update)
	{
		// обновляем все данные
		size_t offset = 0;
		for (size_t index=0; index<mDataInfo.size(); ++index)
		{
			mCurrentData = mData + offset;

			mDataInfo[index].first->doRender(_update || mUpdate);

			offset += mDataInfo[index].second;
		}
	}

	int* RenderItem::getBuffer()
	{
		return mCurrentData;
	}

} // namespace MyGUI
