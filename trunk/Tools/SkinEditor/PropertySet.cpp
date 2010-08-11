/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "PropertySet.h"

namespace tools
{

	PropertySet::PropertySet()
	{
	}

	PropertySet::~PropertySet()
	{
		destroyAllChilds();
	}

	Property* PropertySet::createChild(const MyGUI::UString& _name, const MyGUI::UString& _type, const MyGUI::UString& _value)
	{
		Property* item = new Property(_name, _type);
		item->setValue(_value, "");

		mChilds.push_back(item);
		advise(item);

		return item;
	}

	void PropertySet::destroyChild(Property* _item)
	{
		MYGUI_ASSERT(_item != nullptr, "null reference");

		VectorProperty::iterator item = std::find(mChilds.begin(), mChilds.end(), _item);
		if (item != mChilds.end())
		{
			unadvise(*item);

			delete *item;
			mChilds.erase(item);
		}
		else
		{
			MYGUI_EXCEPT("item not found");
		}
	}

	void PropertySet::destroyChild(const MyGUI::UString& _name)
	{
		Property* item = getChild(_name);
		destroyChild(item);
	}

	void PropertySet::destroyAllChilds()
	{
		while (!mChilds.empty())
			destroyChild(mChilds.back());
	}

	Property* PropertySet::getChild(const MyGUI::UString& _name)
	{
		for (VectorProperty::iterator item=mChilds.begin(); item!=mChilds.end(); ++item)
		{
			if ((*item)->getName() == _name)
				return *item;
		}

		return nullptr;
	}

	EnumeratorProperty PropertySet::getChildsEnumerator()
	{
		return EnumeratorProperty(mChilds);
	}

	void PropertySet::advise(Property* _item)
	{
		_item->eventChangeProperty += MyGUI::newDelegate(this, &PropertySet::notifyChangeProperty);
	}

	void PropertySet::unadvise(Property* _item)
	{
		_item->eventChangeProperty -= MyGUI::newDelegate(this, &PropertySet::notifyChangeProperty);
	}

	void PropertySet::notifyChangeProperty(Property* _item, const MyGUI::UString& _owner)
	{
		eventChangeProperty(_item, _owner);
	}

	void PropertySet::setPropertyValue(const MyGUI::UString& _propertyName, const MyGUI::UString& _value, const MyGUI::UString& _owner)
	{
		Property* proper = getChild(_propertyName);
		if (proper != nullptr)
			proper->setValue(_value, _owner);
	}

	const MyGUI::UString& PropertySet::getPropertyValue(const MyGUI::UString& _propertyName)
	{
		Property* proper = getChild(_propertyName);
		if (proper != nullptr)
			return proper->getValue();

		static MyGUI::UString empty;
		return empty;
	}

	void PropertySet::setPropertyReadOnly(const MyGUI::UString& _propertyName, bool _value)
	{
		Property* proper = getChild(_propertyName);
		if (proper != nullptr)
			proper->setReadOnly(_value);
	}

} // namespace tools
