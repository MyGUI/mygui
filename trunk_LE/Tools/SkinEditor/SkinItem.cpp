/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
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
		skin->setName("Disabled");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");

		skin = mStates.createChild();
		skin->setName("Normal");
		skin->getPropertySet()->setPropertyValue("Visible", "True", "");

		skin = mStates.createChild();
		skin->setName("Over");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");

		skin = mStates.createChild();
		skin->setName("Pressed");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");

		skin = mStates.createChild();
		skin->setName("Selected Disabled");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");

		skin = mStates.createChild();
		skin->setName("Selected Normal");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");

		skin = mStates.createChild();
		skin->setName("Selected Over");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");

		skin = mStates.createChild();
		skin->setName("Selected Pressed");
		skin->getPropertySet()->setPropertyValue("Visible", "False", "");


		SeparatorItem* separator = nullptr;

		separator = mSeparators.createChild();
		separator->setName("Top");
		separator->setCorner(MyGUI::Align::Top);
		separator->getPropertySet()->setPropertyValue("Offset", "6", "");

		separator = mSeparators.createChild();
		separator->setName("Bottom");
		separator->setCorner(MyGUI::Align::Bottom);
		separator->getPropertySet()->setPropertyValue("Offset", "6", "");

		separator = mSeparators.createChild();
		separator->setName("Left");
		separator->setCorner(MyGUI::Align::Left);
		separator->getPropertySet()->setPropertyValue("Offset", "6", "");

		separator = mSeparators.createChild();
		separator->setName("Right");
		separator->setCorner(MyGUI::Align::Right);
		separator->getPropertySet()->setPropertyValue("Offset", "6", "");


		RegionItem* region = nullptr;

		region = mRegions.createChild();
		region->setName("Left Top");
		region->setSeparator(MyGUI::Align::Left | MyGUI::Align::Top);
		region->getPropertySet()->setPropertyValue("Align", "Left Top", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Top");
		region->setSeparator(MyGUI::Align::Top);
		region->getPropertySet()->setPropertyValue("Align", "HStretch Top", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Right Top");
		region->setSeparator(MyGUI::Align::Right | MyGUI::Align::Top);
		region->getPropertySet()->setPropertyValue("Align", "Right Top", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Right");
		region->setSeparator(MyGUI::Align::Right);
		region->getPropertySet()->setPropertyValue("Align", "Right VStretch", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Right Bottom");
		region->setSeparator(MyGUI::Align::Right | MyGUI::Align::Bottom);
		region->getPropertySet()->setPropertyValue("Align", "Right Bottom", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Bottom");
		region->setSeparator(MyGUI::Align::Bottom);
		region->getPropertySet()->setPropertyValue("Align", "HStretch Bottom", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Left Bottom");
		region->setSeparator(MyGUI::Align::Left | MyGUI::Align::Bottom);
		region->getPropertySet()->setPropertyValue("Align", "Left Bottom", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Left");
		region->setSeparator(MyGUI::Align::Left);
		region->getPropertySet()->setPropertyValue("Align", "Left VStretch", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Center");
		region->setSeparator(MyGUI::Align::Center);
		region->getPropertySet()->setPropertyValue("Align", "Stretch", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
		region->getPropertySet()->setPropertyReadOnly("Position", true);
		region->getPropertySet()->setPropertyReadOnly("Align", true);

		region = mRegions.createChild();
		region->setName("Text");
		region->setSeparator(MyGUI::Align::Center);
		region->getPropertySet()->setPropertyValue("Align", "Stretch", "");
		region->getPropertySet()->setPropertyValue("RegionType", "SimpleText", "");
		region->getPropertySet()->setPropertyValue("Visible", "False", "");
		region->getPropertySet()->setPropertyReadOnly("Enabled", true);
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

	void SkinItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);

		ItemHolder<StateItem>::EnumeratorItem stateItems = mStates.getChildsEnumerator();
		while (stateItems.next())
		{
			MyGUI::xml::Element* node = _node->createChild("StateItem");
			stateItems->serialization(node, _version);
		}

		ItemHolder<SeparatorItem>::EnumeratorItem separatorItems = mSeparators.getChildsEnumerator();
		while (separatorItems.next())
		{
			MyGUI::xml::Element* node = _node->createChild("SeparatorItem");
			separatorItems->serialization(node, _version);
		}

		ItemHolder<RegionItem>::EnumeratorItem regionItems = mRegions.getChildsEnumerator();
		while (regionItems.next())
		{
			MyGUI::xml::Element* node = _node->createChild("RegionItem");
			regionItems->serialization(node, _version);
		}

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

	void SkinItem::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		mName = _node->findAttribute("name");

		mStates.destroyAllChilds();
		mSeparators.destroyAllChilds();
		mRegions.destroyAllChilds();

		MyGUI::xml::ElementEnumerator nodes = _node->getElementEnumerator();
		while (nodes.next())
		{
			MyGUI::xml::Element* node = nodes.current();

			if (node->getName() == "PropertySet")
			{
				mPropertySet->deserialization(node, _version);
			}
			else if (node->getName() == "StateItem")
			{
				StateItem* item = mStates.createChild();
				item->deserialization(node, _version);
			}
			else if (node->getName() == "SeparatorItem")
			{
				SeparatorItem* item = mSeparators.createChild();
				item->deserialization(node, _version);
			}
			else if (node->getName() == "RegionItem")
			{
				RegionItem* item = mRegions.createChild();
				item->deserialization(node, _version);
			}
		}
	}

} // namespace tools
