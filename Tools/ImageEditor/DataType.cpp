/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataType.h"

namespace tools
{

	DataType::DataType()
	{
	}

	DataType::~DataType()
	{
	}

	void DataType::deserialization(pugi::xml_node _node)
	{
		mName = _node.select_single_node("Name").node().child_value();
		mFriend = _node.select_single_node("Friend").node().child_value();

		pugi::xpath_node_set childs = _node.select_nodes("Childs/Child/Type");
		for (pugi::xpath_node_set::const_iterator child = childs.begin(); child != childs.end(); child ++)
			mChilds.push_back((*child).node().child_value());

		pugi::xpath_node_set properties = _node.select_nodes("Properties/Property");
		for (pugi::xpath_node_set::const_iterator property = properties.begin(); property != properties.end(); property ++)
		{
			DataTypePropertyPtr info(new DataTypeProperty());
			info->deserialization((*property).node());
			mProperties.push_back(info);
		}
	}

	const std::string& DataType::getName() const
	{
		return mName;
	}

	const std::string& DataType::getFriend() const
	{
		return mFriend;
	}

	const DataType::VectorString& DataType::getChilds() const
	{
		return mChilds;
	}

	const DataType::VectorProperty& DataType::getProperties() const
	{
		return mProperties;
	}

	bool DataType::isChild(const std::string& _child) const
	{
		for (VectorString::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			if ((*child) == _child)
				return true;
		}

		return false;
	}

}
