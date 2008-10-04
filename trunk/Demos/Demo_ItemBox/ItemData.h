/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_DATA_H__
#define __ITEM_DATA_H__

#include <Ogre.h>
#include "ResourceItemInfo.h"
#include "MyGUI_ResourceManager.h"

enum ItemType
{
	TypeNone,
	TypeRoll,
	TypeCloth,
	TypeTear,
	TypeEye,
	TypeEmerald,
	TypeWings,
	TypeIce,
	TypeBoard,
	TypeBoots
};

class ItemData
{
public:
	ItemData(const std::string & _resource, ItemType _type, size_t _count) :
		type(_type),
		count(_count)
	{
		MyGUI::ResourceManager & manager = MyGUI::ResourceManager::getInstance();
		mResourceInfo = manager.getResource(_resource)->castType<demo::ResourceItemInfo>();
		mResourceImage = manager.getResource(mResourceInfo->getItemResourceImage())->castType<MyGUI::ResourceImageSet>();
	}

	size_t count;
	ItemType type;

	demo::ResourceItemInfoPtr mResourceInfo;
	MyGUI::ResourceImageSetPtr mResourceImage;
};

#endif // __ITEM_DATA_H__
