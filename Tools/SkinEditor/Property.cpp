/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "Property.h"

namespace tools
{

	Property::Property(const MyGUI::UString& _name, const MyGUI::UString& _type) :
		mName(_name),
		mType(_type)
	{
	}

	Property::~Property()
	{
	}

	const MyGUI::UString& Property::getValue()
	{
		return mValue;
	}

	const MyGUI::UString& Property::getName()
	{
		return mName;
	}

	const MyGUI::UString& Property::getType()
	{
		return mType;
	}

	void Property::setValue(const MyGUI::UString& _value, const MyGUI::UString& _owner)
	{
		mValue = _value;
		eventChangeProperty(this, _owner);
	}

} // namespace tools
