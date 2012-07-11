/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "Precompiled.h"
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

	const MyGUI::UString& SettingsSector::getName() const
	{
		return mName;
	}

	void SettingsSector::setName(const MyGUI::UString& _value)
	{
		mName = _value;
	}

	bool SettingsSector::getExistProperty(const MyGUI::UString& _propertyName)
	{
		MapUString::const_iterator item = mProperties.find(_propertyName);
		return item != mProperties.end();
	}

	const MyGUI::UString& SettingsSector::getPropertyValue(const MyGUI::UString& _propertyName)
	{
		MapUString::const_iterator item = mProperties.find(_propertyName);
		if (item != mProperties.end())
			return (*item).second;

		return MyGUI::Constants::getEmptyUString();
	}

	void SettingsSector::setPropertyValue(const MyGUI::UString& _propertyName, const MyGUI::UString& _propertyValue)
	{
		MapUString::iterator item = mProperties.find(_propertyName);
		if (item != mProperties.end())
			(*item).second = _propertyValue;
		else
			mProperties.insert(std::make_pair(_propertyName, _propertyValue));

		eventSettingsChanged(this, _propertyName);
	}

	void SettingsSector::setPropertyValue(const MyGUI::UString& _propertyName, const std::string& _propertyValue)
	{
		setPropertyValue(_propertyName, MyGUI::UString(_propertyValue));
	}

	void SettingsSector::setPropertyValueList(const MyGUI::UString& _propertyName, const VectorUString& _propertyValues)
	{
		clearProperty(_propertyName);
		for (size_t index = 0; index < _propertyValues.size(); ++ index)
			setPropertyValue(MyGUI::utility::toString(_propertyName, '.', index), _propertyValues[index]);
	}

	void SettingsSector::setPropertyValueList(const MyGUI::UString& _propertyName, const VectorString& _propertyValues)
	{
		clearProperty(_propertyName);
		for (size_t index = 0; index < _propertyValues.size(); ++ index)
			setPropertyValue(MyGUI::utility::toString(_propertyName, '.', index), _propertyValues[index]);
	}

	void SettingsSector::clearProperty(const MyGUI::UString& _propertyName)
	{
		for (MapUString::iterator item = mProperties.begin(); item != mProperties.end(); /* inside loop */)
		{
			size_t indexSeparator = (*item).first.find('.');
			if (indexSeparator != MyGUI::UString::npos)
			{
				if ((*item).first.substr(0, indexSeparator) == _propertyName)
				{
					mProperties.erase(item++);
				}
				else
				{
					++item;
				}
			}
			else
			{
				if ((*item).first == _propertyName)
				{
					mProperties.erase(item++);
				}
				else
				{
					++item;
				}
			}
		}
	}

	SettingsSector::VectorUString SettingsSector::getPropertyValueList(const MyGUI::UString& _propertyName)
	{
		VectorUString result;

		for (MapUString::iterator item = mProperties.begin(); item != mProperties.end(); ++ item)
		{
			size_t indexSeparator = (*item).first.find('.');
			if (indexSeparator != MyGUI::UString::npos)
			{
				if ((*item).first.substr(0, indexSeparator) == _propertyName)
					result.push_back((*item).second);
			}
		}

		return result;
	}
	
} // namespace tools
