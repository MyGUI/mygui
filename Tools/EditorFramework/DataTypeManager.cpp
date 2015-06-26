/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "DataTypeManager.h"
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
			for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			{
				DataTypePtr data = DataTypePtr(new DataType());
				data->deserialization((*node).node());
				mDataInfos.push_back(data);
			}
		}
	}

	void DataTypeManager::clear()
	{
		mDataInfos.clear();
	}

	DataTypePtr DataTypeManager::getType(const std::string& _type)
	{
		for (VectorDataInfo::const_iterator data = mDataInfos.begin(); data != mDataInfos.end(); data ++)
		{
			if ((*data)->getName() == _type)
				return *data;
		}

		return nullptr;
	}

	DataTypePtr DataTypeManager::getParentType(const std::string& _type)
	{
		for (VectorDataInfo::const_iterator data = mDataInfos.begin(); data != mDataInfos.end(); data ++)
		{
			if ((*data)->isChild(_type))
				return *data;
		}

		return nullptr;
	}

}
