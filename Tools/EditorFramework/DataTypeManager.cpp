/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataTypeManager.h"

#include <memory>
#include "pugixml.hpp"
#include "MyGUI_DataManager.h"

namespace tools
{

	DataTypeManager* DataTypeManager::mInstance = nullptr;

	DataTypeManager::DataTypeManager()
	{
		mInstance = this;
	}

	DataTypeManager::~DataTypeManager()
	{
		mInstance = nullptr;
	}

	DataTypeManager& DataTypeManager::getInstance()
	{
		return *mInstance;
	}

	DataTypeManager* DataTypeManager::getInstancePtr()
	{
		return mInstance;
	}

	void DataTypeManager::initialise()
	{
	}

	void DataTypeManager::shutdown()
	{
		clear();
	}

	void DataTypeManager::load(const std::string& _fileName)
	{
		std::string fileName = MyGUI::DataManager::getInstance().getDataPath(_fileName);
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(fileName.c_str());
		if (result)
		{
			pugi::xpath_node_set nodes = doc.select_nodes("Document/DataTypes/DataType");
			for (const auto& node : nodes)
			{
				DataTypePtr data = std::make_shared<DataType>();
				data->deserialization(node.node());
				mDataInfos.push_back(data);
			}
		}
	}

	void DataTypeManager::clear()
	{
		mDataInfos.clear();
	}

	DataTypePtr DataTypeManager::getType(std::string_view _type)
	{
		for (const auto& dataInfo : mDataInfos)
		{
			if (dataInfo->getName() == _type)
				return dataInfo;
		}

		return nullptr;
	}

	DataTypePtr DataTypeManager::getParentType(std::string_view _type)
	{
		for (const auto& dataInfo : mDataInfos)
		{
			if (dataInfo->isChild(_type))
				return dataInfo;
		}

		return nullptr;
	}

}
