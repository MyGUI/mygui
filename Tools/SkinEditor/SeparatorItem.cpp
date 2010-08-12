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
		mPropertySet(nullptr),
		mCorner(MyGUI::Align::Center)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Visible", "Bool", "False");
		mPropertySet->createChild("Position", "Int", "0");
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
		return mCorner;
	}

	void SeparatorItem::setCorner(MyGUI::Align _value)
	{
		mCorner = _value;
	}

	void SeparatorItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);
		_node->createChild("Corner", mCorner.print());

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

	void SeparatorItem::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
	}

} // namespace tools
