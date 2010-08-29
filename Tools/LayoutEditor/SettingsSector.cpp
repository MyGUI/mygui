/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "SettingsSector.h"

namespace tools
{

	SettingsSector::SettingsSector()
	{
	}

	SettingsSector::~SettingsSector()
	{
	}

	void SettingsSector::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		MyGUI::xml::Element* element = _node->createChild(mName);

		for (MapUString::const_iterator item = mProperties.begin(); item != mProperties.end(); ++item)
		{
			MyGUI::xml::Element* prop = element->createChild("Property");
			prop->addAttribute("key", (*item).first);
			prop->addAttribute("value", (*item).second);
		}
	}

	void SettingsSector::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		mName = _node->getName();

		MyGUI::xml::ElementEnumerator prop = _node->getElementEnumerator();
		while (prop.next("Property"))
		{
			std::string key;
			if (!prop->findAttribute("key", key))
				continue;

			std::string value;
			if (!prop->findAttribute("value", value))
				continue;

			mProperties[key] = value;
		}
	}

	const MyGUI::UString& SettingsSector::getName()
	{
		return mName;
	}

	void SettingsSector::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	bool SettingsSector::getExistProperty(const MyGUI::UString& _propertName)
	{
		MapUString::const_iterator item = mProperties.find(_propertName);
		return item != mProperties.end();
	}

	const MyGUI::UString& SettingsSector::getPropertyValue(const MyGUI::UString& _propertName)
	{
		MapUString::const_iterator item = mProperties.find(_propertName);
		if (item != mProperties.end())
			return (*item).second;

		static MyGUI::UString empty;
		return empty;
	}

	void SettingsSector::setPropertyValue(const MyGUI::UString& _propertName, const MyGUI::UString& _propertValue)
	{
		MapUString::iterator item = mProperties.find(_propertName);
		if (item != mProperties.end())
			(*item).second = _propertValue;
		else
			mProperties.insert(std::make_pair(_propertName, _propertValue));
	}

} // namespace tools
