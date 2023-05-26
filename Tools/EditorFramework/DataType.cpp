/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataType.h"

namespace tools
{

	void DataType::deserialization(pugi::xml_node _node)
	{
		mName = _node.select_single_node("Name").node().child_value();
		mFriend = _node.select_single_node("Friend").node().child_value();

		pugi::xpath_node_set childs = _node.select_nodes("Childs/Child/Type");
		for (const auto& child : childs)
			mChilds.emplace_back(child.node().child_value());

		pugi::xpath_node_set properties = _node.select_nodes("Properties/Property");
		for (const auto& property : properties)
		{
			DataTypePropertyPtr info(new DataTypeProperty());
			info->deserialization(property.node());
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

	bool DataType::isChild(std::string_view _child) const
	{
		for (const auto& child : mChilds)
		{
			if (child == _child)
				return true;
		}

		return false;
	}

}
