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

	Property::Property(DataTypePropertyPtr _type, DataPtr _owner) :
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
			eventChangeProperty(mWeakThis.lock());
		}
	}

	DataTypePropertyPtr Property::getType()
	{
		return mType;
	}

	DataPtr Property::getOwner()
	{
		return mOwner;
	}

	void Property::initialise()
	{
		if (!mType->getInitialisator().empty())
		{
			IPropertyInitialisator* initialisator = components::FactoryManager::GetInstance().CreateItem<IPropertyInitialisator>(mType->getInitialisator());
			if (initialisator != nullptr)
				initialisator->initialise(mWeakThis.lock());
		}
		else
		{
			mValue = mType->getDefaultValue();
		}
	}

	PropertyPtr Property::CreateInstance(DataTypePropertyPtr _type, DataPtr _owner)
	{
		PropertyPtr result = PropertyPtr(new Property(_type, _owner));
		result->mWeakThis = PropertyWeak(result);
		return result;
	}

}
