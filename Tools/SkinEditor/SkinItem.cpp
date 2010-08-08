/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SkinItem.h"

namespace tools
{

	SkinItem::SkinItem() :
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Texture", "Texture");
		mPropertySet->createChild("Coord", "IntCoord", "0 0 32 32");

		StateItem* item = nullptr;

		item = mStates.createChild();
		item->setName("disabled");
		item = mStates.createChild();
		item->setName("normal");
		item = mStates.createChild();
		item->setName("over");
		item = mStates.createChild();
		item->setName("pressed");
		item = mStates.createChild();
		item->setName("selected disabled");
		item = mStates.createChild();
		item->setName("selected normal");
		item = mStates.createChild();
		item->setName("selected over");
		item = mStates.createChild();
		item->setName("selected pressed");
	}

	SkinItem::~SkinItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& SkinItem::getName()
	{
		return mName;
	}

	void SkinItem::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	PropertySet* SkinItem::getPropertySet()
	{
		return mPropertySet;
	}

} // namespace tools
