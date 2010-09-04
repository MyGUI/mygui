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
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Position", "IntCoord", "0 0 32 32");
		mPropertySet->createChild("Visible", "Bool", "True");
		mPropertySet->createChild("Enabled", "Bool", "True");
		mPropertySet->createChild("Align", "Align", "Default");
		mPropertySet->createChild("RegionType", "RegionType", "SubSkin");
		mPropertySet->createChild("Separator", "", "Center");
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

	MyGUI::Align RegionItem::getSeparator()
	{
		return MyGUI::Align::parse(mPropertySet->getPropertyValue("Separator"));
	}

	void RegionItem::setSeparator(MyGUI::Align _value)
	{
		mPropertySet->setPropertyValue("Separator", _value.print(), "");
	}

	PropertySet* RegionItem::getPropertySet()
	{
		return mPropertySet;
	}

	void RegionItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

	void RegionItem::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		mName = _node->findAttribute("name");

		MyGUI::xml::ElementEnumerator nodes = _node->getElementEnumerator();
		while (nodes.next())
		{
			MyGUI::xml::Element* node = nodes.current();

			if (node->getName() == "PropertySet")
			{
				mPropertySet->deserialization(node, _version);
			}
		}
	}

} // namespace tools
