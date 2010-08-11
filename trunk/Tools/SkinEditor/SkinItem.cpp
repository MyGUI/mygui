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
		separator->setCorner(MyGUI::Align::Top);
		separator = mSeparators.createChild();
		separator->setName("bottom");
		separator->setCorner(MyGUI::Align::Bottom);
		separator = mSeparators.createChild();
		separator->setName("left");
		separator->setCorner(MyGUI::Align::Left);
		separator = mSeparators.createChild();
		separator->setName("right");
		separator->setCorner(MyGUI::Align::Right);

		RegionItem* region = nullptr;
		region = mRegions.createChild();
		region->setName("left top");
		region->setSeparator(MyGUI::Align::Left | MyGUI::Align::Top);
		region = mRegions.createChild();
		region->setName("top");
		region->setSeparator(MyGUI::Align::Top);
		region = mRegions.createChild();
		region->setName("right top");
		region->setSeparator(MyGUI::Align::Right | MyGUI::Align::Top);
		region = mRegions.createChild();
		region->setName("right");
		region->setSeparator(MyGUI::Align::Right);
		region = mRegions.createChild();
		region->setName("right bottom");
		region->setSeparator(MyGUI::Align::Right | MyGUI::Align::Bottom);
		region = mRegions.createChild();
		region->setName("bottom");
		region->setSeparator(MyGUI::Align::Bottom);
		region = mRegions.createChild();
		region->setName("left bottom");
		region->setSeparator(MyGUI::Align::Left | MyGUI::Align::Bottom);
		region = mRegions.createChild();
		region->setName("left");
		region->setSeparator(MyGUI::Align::Left);
		region = mRegions.createChild();
		region->setName("center");
		region->setSeparator(MyGUI::Align::Center);
		region = mRegions.createChild();
		region->setName("text");
		region->setSeparator(MyGUI::Align::Center);
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
