/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
#include "SeparatorItem.h"

namespace tools
{

	/*SeparatorItem::SeparatorItem() :
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

	const MyGUI::UString& SeparatorItem::getName() const
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

*/
} // namespace tools
