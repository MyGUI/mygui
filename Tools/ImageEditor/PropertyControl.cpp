/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/

#include "Precompiled.h"
#include "PropertyControl.h"

namespace tools
{

	PropertyControl::PropertyControl() :
		mProperty(nullptr)
	{
	}

	PropertyControl::~PropertyControl()
	{
	}

	void PropertyControl::setProperty(Property* _value)
	{
		unadvice();
		mProperty = _value;
		advice();

		updateCaption();
		updateProperty();
	}

	Property* PropertyControl::getProperty()
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

	void PropertyControl::notifyChangeProperty(Property* _sender)
	{
		updateProperty();
	}

	void PropertyControl::updateCaption()
	{
	}

}
