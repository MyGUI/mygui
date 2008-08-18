/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_DATA_H__
#define __ITEM_DATA_H__

#include <Ogre.h>

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
	ItemData(ItemType _type, size_t _count, const Ogre::UTFString & _name, const Ogre::UTFString & _description) :
		type(_type),
		count(_count),
		name(_name),
		description(_description)
	{
	}

	size_t count;
	ItemType type;

	Ogre::UTFString name;
	Ogre::UTFString description;
};

#endif // __ITEM_DATA_H__
