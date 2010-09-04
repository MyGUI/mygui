/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertyControl.h"

namespace tools
{

	PropertyControl::PropertyControl() :
		mProperty(nullptr)
	{
		mTypeName = MyGUI::utility::toString((int)this);
	}

	PropertyControl::~PropertyControl()
	{
	}

	void PropertyControl::setProperty(Property* _value)
	{
		unadvice();
		mProperty = _value;
		advice();

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
			mProperty->eventChangeProperty += MyGUI::newDelegate(this, &PropertyControl::notifyChangeProperty);
		}
	}

	void PropertyControl::unadvice()
	{
		if (mProperty != nullptr)
		{
			mProperty->eventChangeProperty -= MyGUI::newDelegate(this, &PropertyControl::notifyChangeProperty);
			mProperty = nullptr;
		}
	}

	void PropertyControl::updateProperty()
	{
	}

	void PropertyControl::notifyChangeProperty(Property* _sender, const MyGUI::UString& _owner)
	{
		if (_owner != mTypeName)
			updateProperty();
	}

	const MyGUI::UString& PropertyControl::getTypeName()
	{
		return mTypeName;
	}

} // namespace tools
