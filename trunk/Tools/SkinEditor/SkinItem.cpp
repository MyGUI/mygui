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

		StateItem* skin = nullptr;
		skin = mStates.createChild();
		skin->setName("disabled");
		skin = mStates.createChild();
		skin->setName("normal");
		skin = mStates.createChild();
		skin->setName("over");
		skin = mStates.createChild();
		skin->setName("pressed");
		skin = mStates.createChild();
		skin->setName("selected disabled");
		skin = mStates.createChild();
		skin->setName("selected normal");
		skin = mStates.createChild();
		skin->setName("selected over");
		skin = mStates.createChild();
		skin->setName("selected pressed");

		SeparatorItem* separator = nullptr;
		separator = mSeparators.createChild();
		separator->setName("top");
		separator->setHorizontal(true);
		separator = mSeparators.createChild();
		separator->setName("bottom");
		separator->setHorizontal(true);
		separator = mSeparators.createChild();
		separator->setName("left");
		separator->setHorizontal(false);
		separator = mSeparators.createChild();
		separator->setName("right");
		separator->setHorizontal(false);

		RegionItem* region = nullptr;
		region = mRegions.createChild();
		region->setName("left top");
		region = mRegions.createChild();
		region->setName("top");
		region = mRegions.createChild();
		region->setName("right top");
		region = mRegions.createChild();
		region->setName("right");
		region = mRegions.createChild();
		region->setName("right bottom");
		region = mRegions.createChild();
		region->setName("bottom");
		region = mRegions.createChild();
		region->setName("left bottom");
		region = mRegions.createChild();
		region->setName("left");
		region = mRegions.createChild();
		region->setName("center");
		region = mRegions.createChild();
		region->setName("text");
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
