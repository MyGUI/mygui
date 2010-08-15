/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#include "precompiled.h"
#include "Property.h"
#include "ActionManager.h"

namespace tools
{

	Property::Property(const MyGUI::UString& _name, const MyGUI::UString& _type) :
		mName(_name),
		mType(_type),
		mReadOnly(false)
	{
	}

	Property::~Property()
	{
	}

	const MyGUI::UString& Property::getValue()
	{
		return mValue;
	}

	const MyGUI::UString& Property::getName()
	{
		return mName;
	}

	const MyGUI::UString& Property::getType()
	{
		return mType;
	}

	void Property::setValue(const MyGUI::UString& _value, const MyGUI::UString& _owner)
	{
		if (mValue != _value)
		{
			mValue = _value;
			eventChangeProperty(this, _owner);

			ActionManager::getInstance().setChanges(true);
		}
	}

	bool Property::getReadOnly()
	{
		return mReadOnly;
	}

	void Property::setReadOnly(bool _value)
	{
		mReadOnly = _value;
	}

	void Property::serialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		_node->addAttribute("name", mName);
		_node->addAttribute("type", mType);
		_node->addAttribute("read_only", mReadOnly ? "True" : "False");
		_node->setContent(mValue);
	}

	void Property::deserialization(MyGUI::xml::Element* _node, MyGUI::Version _version)
	{
		mName = _node->findAttribute("name");
		mType = _node->findAttribute("type");
		mReadOnly = _node->findAttribute("read_only") == "True";
		mValue = _node->getContent();
	}

} // namespace tools
