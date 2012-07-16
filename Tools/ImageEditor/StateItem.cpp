/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "StateItem.h"

/*namespace tools
{

	StateItem::StateItem() :
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Visible", "Bool", "True");
		mPropertySet->createChild("Position", "IntPoint", "0 0");
		mPropertySet->createChild("TextColour", "Colour", "0 0 0");
		mPropertySet->createChild("TextShift", "Int", "0");
	}

	StateItem::~StateItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& StateItem::getName() const
	{
		return mName;
	}

	void StateItem::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	PropertySet* StateItem::getPropertySet()
	{
		return mPropertySet;
	}

	void StateItem::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);

		MyGUI::xml::Element* node = _node->createChild("PropertySet");
		mPropertySet->serialization(node, _version);
	}

}*/
