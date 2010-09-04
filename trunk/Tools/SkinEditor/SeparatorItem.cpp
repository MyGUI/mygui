/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SeparatorItem.h"

namespace tools
{

	SeparatorItem::SeparatorItem() :
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Visible", "Bool", "False");
		mPropertySet->createChild("Offset", "Int", "0");
		mPropertySet->createChild("Corner", "", "Center");
	}

	SeparatorItem::~SeparatorItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& SeparatorItem::getName()
	{
		return mName;
	}

	void SeparatorItem::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	PropertySet* SeparatorItem::getPropertySet()
	{
		return mPropertySet;
	}

	MyGUI::Align SeparatorItem::getCorner()
	{
		return MyGUI::Align::parse(mPropertySet->getPropertyValue("Corner"));
	}

	void SeparatorItem::setCorner(MyGUI::Align _value)
	{
		mPropertySet->setPropertyValue("Corner", _value.print(), "");
	}

	void SeparatorItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

	void SeparatorItem::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
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
