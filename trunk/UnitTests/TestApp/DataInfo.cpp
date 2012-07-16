/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "DataInfo.h"

namespace tools
{
	DataInfo::DataInfo()
	{
	}

	DataInfo::~DataInfo()
	{
		for (VectorProperty::iterator property = mProperties.begin(); property != mProperties.end(); property ++)
			delete *property;
		mProperties.clear();
	}

	void DataInfo::deserialization(pugi::xml_node _node)
	{
		mType = _node.select_single_node("Type").node().child_value();

		pugi::xpath_node_set childs = _node.select_nodes("Childs/Child/Type");
		for (pugi::xpath_node_set::const_iterator child = childs.begin(); child != childs.end(); child ++)
			mChilds.push_back((*child).node().child_value());

		pugi::xpath_node_set properties = _node.select_nodes("Properties/Property");
		for (pugi::xpath_node_set::const_iterator property = properties.begin(); property != properties.end(); property ++)
		{
			DataPropertyInfo* info = new DataPropertyInfo();
			info->deserialization((*property).node());
			mProperties.push_back(info);
		}
	}

	const std::string& DataInfo::getType() const
	{
		return mType;
	}

	const DataInfo::VectorString& DataInfo::getChilds() const
	{
		return mChilds;
	}

	const DataInfo::VectorProperty& DataInfo::getProperties() const
	{
		return mProperties;
	}
}
