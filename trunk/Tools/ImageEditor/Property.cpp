/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "Property.h"

namespace tools
{

	Property::Property(DataTypeProperty* _type) :
		mType(_type)
	{
	}

	Property::~Property()
	{
	}

	const std::string& Property::getValue() const
	{
		return mValue;
	}

	void Property::setValue(const std::string& _value)
	{
		if (mValue != _value)
		{
			mValue = _value;
			eventChangeProperty(this);
		}
	}

	DataTypeProperty* Property::getType()
	{
		return mType;
	}

}
