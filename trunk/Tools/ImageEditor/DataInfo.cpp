/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataInfo.h"

namespace tools
{

	DataType::DataType()
	{
	}

	DataType::~DataType()
	{
		for (VectorProperty::iterator property = mProperties.begin(); property != mProperties.end(); property ++)
			delete *property;
		mProperties.clear();
	}

	void DataType::deserialization(pugi::xml_node _node)
	{
		mName = _node.select_single_node("Name").node().child_value();

		pugi::xpath_node_set childs = _node.select_nodes("Childs/Child/Type");
		for (pugi::xpath_node_set::const_iterator child = childs.begin(); child != childs.end(); child ++)
			mChilds.push_back((*child).node().child_value());

		pugi::xpath_node_set properties = _node.select_nodes("Properties/Property");
		for (pugi::xpath_node_set::const_iterator property = properties.begin(); property != properties.end(); property ++)
		{
			DataTypeProperty* info = new DataTypeProperty();
			info->deserialization((*property).node());
			mProperties.push_back(info);
		}
	}

	const std::string& DataType::getName() const
	{
		return mName;
	}

	const DataType::VectorString& DataType::getChilds() const
	{
		return mChilds;
	}

	const DataType::VectorProperty& DataType::getProperties() const
	{
		return mProperties;
	}

	bool DataType::isChild(DataType* _child)
	{
		for (VectorString::const_iterator child = mChilds.begin(); child != mChilds.end(); child ++)
		{
			if ((*child) == _child->getName())
				return true;
		}

		return false;
	}

}
