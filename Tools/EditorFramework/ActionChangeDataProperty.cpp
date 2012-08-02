/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "ActionChangeDataProperty.h"
#include "DataManager.h"
#include "DataSelectorManager.h"

namespace tools
{

	ActionChangeDataProperty::ActionChangeDataProperty() :
		mProperty(nullptr)
	{
	}

	ActionChangeDataProperty::~ActionChangeDataProperty()
	{
	}

	void ActionChangeDataProperty::setProperty(PropertyPtr _value)
	{
		mProperty = _value;
	}

	PropertyPtr ActionChangeDataProperty::getProperty()
	{
		return mProperty;
	}

	void ActionChangeDataProperty::setValue(const std::string& _value)
	{
		mValue = _value;
	}

	const std::string& ActionChangeDataProperty::getValue() const
	{
		return mValue;
	}

}
