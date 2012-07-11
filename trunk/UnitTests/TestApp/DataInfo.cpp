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
	}

	void DataInfo::deserialization(pugi::xml_node _node)
	{
		mType = _node.select_single_node("Type").node().child_value();

		pugi::xpath_node_set childs = _node.select_nodes("Childs/Child/Type");
		for (pugi::xpath_node_set::const_iterator child = childs.begin(); child != childs.end(); child ++)
			mChilds.push_back((*child).node().child_value());
	}

	const std::string& DataInfo::getType() const
	{
		return mType;
	}


	const DataInfo::VectorString& DataInfo::getChilds() const
	{
		return mChilds;
	}
}
