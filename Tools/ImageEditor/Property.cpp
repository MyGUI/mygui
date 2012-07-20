/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "Property.h"
#include "Data.h"
#include "IPropertyInitialisator.h"
#include "FactoryManager.h"

namespace tools
{

	Property::Property(DataTypeProperty* _type, Data* _owner) :
		mType(_type),
		mOwner(_owner)
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

	Data* Property::getOwner()
	{
		return mOwner;
	}

	void Property::initialise()
	{
		if (!mType->getInitialisator().empty())
		{
			IPropertyInitialisator* initialisator = components::FactoryManager::GetInstance().CreateItem<IPropertyInitialisator>(mType->getInitialisator());
			if (initialisator != nullptr)
				initialisator->initialise(this);
		}
		else
		{
			mValue = mType->getDefaultValue();
		}
	}

}
