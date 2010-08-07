/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "StateItem.h"

namespace tools
{

	StateItem::StateItem() :
		mPropertySet(nullptr)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Visible", "Bool", "True");
		mPropertySet->createChild("Position", "IntPoint", "0 0");
	}

	StateItem::~StateItem()
	{
		delete mPropertySet;
	}

	const MyGUI::UString& StateItem::getName()
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

} // namespace tools
