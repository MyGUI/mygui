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
		mPropertySet->createChild("RegionType", "RegionType", "SubSkin");
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

	void RegionItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->createChild("Name", mName);
		_node->createChild("Separator", mSeparator.print());

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

	void RegionItem::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
	}

} // namespace tools
