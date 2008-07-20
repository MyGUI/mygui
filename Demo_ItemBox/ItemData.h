/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_DATA_H__
#define __ITEM_DATA_H__

class ItemData2
{
public:
	ItemData2(size_t _count, size_t _type, const Ogre::UTFString & _name, const Ogre::UTFString & _description) :
		count(_count),
		type(_type),
		name(_name),
		description(_description)
	{
	}

	size_t count;
	size_t type;

	Ogre::UTFString name;
	Ogre::UTFString description;
};

#endif // __ITEM_DATA_H__
