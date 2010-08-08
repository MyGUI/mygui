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
		mHorizontal(false)
	{
		mPropertySet = new PropertySet();
		mPropertySet->createChild("Visible", "Bool", "True");
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

	bool SeparatorItem::getHorizontal()
	{
		return mHorizontal;
	}

	void SeparatorItem::setHorizontal(bool _value)
	{
		mHorizontal = _value;
	}

} // namespace tools
