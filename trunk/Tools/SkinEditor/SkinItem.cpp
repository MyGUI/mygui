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
		separator->setName("left horizontal");
		separator = mSeparators.createChild();
		separator->setName("right horizontal");
		separator = mSeparators.createChild();
		separator->setName("top vertical");
		separator = mSeparators.createChild();
		separator->setName("bottom vertical");
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
