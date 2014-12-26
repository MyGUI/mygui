/*!
	@file
	@author		Albert Semenov
	@date		07/2008
*/
#ifndef ITEM_DATA_H_
#define ITEM_DATA_H_

#include "ResourceItemInfo.h"
#include "MyGUI_ResourceManager.h"

namespace demo
{

	class ItemData
	{
	public:
		ItemData() :
			count(0),
			mResourceInfo(nullptr),
			mResourceImage(nullptr)
		{
		}

		ItemData(const std::string& _resource, size_t _count) :
			count(_count),
			mResourceInfo(nullptr),
			mResourceImage(nullptr)
		{
			MyGUI::ResourceManager& manager = MyGUI::ResourceManager::getInstance();
			mResourceInfo = manager.getByName(_resource)->castType<demo::ResourceItemInfo>();
			mResourceImage = manager.getByName(mResourceInfo->getItemResourceImage())->castType<MyGUI::ResourceImageSet>();
		}

		bool isEmpty() const
		{
			return mResourceInfo == 0;
		}

		bool compare(ItemData* _data) const
		{
			return mResourceInfo == _data->mResourceInfo;
		}

		void add(ItemData* _data)
		{
			count += _data->count;
			mResourceInfo = _data->mResourceInfo;
			mResourceImage = _data->mResourceImage;
		}

		void clear()
		{
			mResourceInfo = nullptr;
			mResourceImage = nullptr;
			count = 0;
		}

		size_t getCount() const
		{
			return count;
		}

		demo::ResourceItemInfoPtr getInfo() const
		{
			return mResourceInfo;
		}

		MyGUI::ResourceImageSetPtr getImage() const
		{
			return mResourceImage;
		}

	private:
		size_t count;
		demo::ResourceItemInfoPtr mResourceInfo;
		MyGUI::ResourceImageSetPtr mResourceImage;
	};

} // namespace demo

#endif // ITEM_DATA_H_
