/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyControl.h"
#include "PropertyUtility.h"

namespace tools
{

	PropertyControl::PropertyControl() :
		mProperty(nullptr)
	{
	}

	void PropertyControl::setProperty(PropertyPtr _value)
	{
		unadvice();
		mProperty = _value;
		advice();

		updateCaption();
		updateProperty();
	}

	PropertyPtr PropertyControl::getProperty()
	{
		return mProperty;
	}

	void PropertyControl::advice()
	{
		if (mProperty != nullptr)
		{
			mProperty->eventChangeProperty.connect(this, &PropertyControl::notifyChangeProperty);
		}
	}

	void PropertyControl::unadvice()
	{
		if (mProperty != nullptr)
		{
			mProperty->eventChangeProperty.disconnect(this);
			mProperty = nullptr;
		}
	}

	void PropertyControl::updateProperty()
	{
	}

	void PropertyControl::notifyChangeProperty(PropertyPtr _sender)
	{
		updateProperty();
	}

	void PropertyControl::updateCaption()
	{
	}

	void PropertyControl::executeAction(std::string_view _value, bool _merge)
	{
		PropertyUtility::executeAction(mProperty, _value, _merge);
	}

}
