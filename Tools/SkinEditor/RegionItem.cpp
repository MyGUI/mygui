/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "RegionItem.h"

namespace tools
{

	RegionItem::RegionItem() :
		mPropertySet(nullptr),
		mSeparator(MyGUI::Align::Center)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Position", "IntCoord", "0 0 32 32");
		mPropertySet->createChild("Visible", "Bool", "True");
		mPropertySet->createChild("Enabled", "Bool", "True");
		mPropertySet->createChild("Align", "Align", "Default");
		mPropertySet->createChild("Type", "RegionType", "Texture");
	}

	RegionItem::~RegionItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& RegionItem::getName()
	{
		return mName;
	}

	void RegionItem::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	PropertySet* RegionItem::getPropertySet()
	{
		return mPropertySet;
	}

} // namespace tools
