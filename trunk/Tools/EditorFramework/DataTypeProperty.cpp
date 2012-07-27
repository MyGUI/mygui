/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataTypeProperty.h"

namespace tools
{

	DataTypeProperty::DataTypeProperty() :
		mReadOnly(false),
		mVisible(false)
	{
	}

	DataTypeProperty::~DataTypeProperty()
	{
	}

	void DataTypeProperty::deserialization(pugi::xml_node _node)
	{
		mName = _node.select_single_node("Name").node().child_value();
		mType = _node.select_single_node("Type").node().child_value();
		mDefaultValue = _node.select_single_node("Default").node().child_value();
		mInitialisator = _node.select_single_node("Initialisator").node().child_value();
		mReadOnly = MyGUI::utility::parseValue<bool>(_node.select_single_node("ReadOnly").node().child_value());
		mVisible = MyGUI::utility::parseValue<bool>(_node.select_single_node("Visible").node().child_value());
		mAction = _node.select_single_node("Action").node().child_value();
	}

	const std::string& DataTypeProperty::getName() const
	{
		return mName;
	}

	const std::string& DataTypeProperty::getType() const
	{
		return mType;
	}

	const std::string& DataTypeProperty::getDefaultValue() const
	{
		return mDefaultValue;
	}

	const std::string& DataTypeProperty::getInitialisator() const
	{
		return mInitialisator;
	}

	bool DataTypeProperty::getReadOnly() const
	{
		return mReadOnly;
	}

	bool DataTypeProperty::getVisible() const
	{
		return mVisible;
	}

	const std::string& DataTypeProperty::getAction() const
	{
		return mAction;
	}

}
