/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "DataInfoManager.h"
#include "pugixml.hpp"
#include "MyGUI_DataManager.h"

namespace tools
{
	DataInfoManager* DataInfoManager::mInstance = nullptr;

	DataInfoManager::DataInfoManager()
	{
		mInstance = this;
	}

	DataInfoManager::~DataInfoManager()
	{
		mInstance = nullptr;
	}

	DataInfoManager& DataInfoManager::getInstance()
	{
		return *mInstance;
	}

	DataInfoManager* DataInfoManager::getInstancePtr()
	{
		return mInstance;
	}

	void DataInfoManager::initialise()
	{
	}

	void DataInfoManager::shutdown()
	{
		clear();
	}

	void DataInfoManager::load(const std::string& _fileName)
	{
		std::string fileName = MyGUI::DataManager::getInstance().getDataPath(_fileName);
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file(fileName.c_str());
		if (result)
		{
			pugi::xpath_node_set nodes = doc.select_nodes("Document/DataInfos/DataInfo");
			for (pugi::xpath_node_set::const_iterator node = nodes.begin(); node != nodes.end(); node ++)
			{
				DataInfo* data = new DataInfo();
				data->deserialization((*node).node());
				mDataInfos.push_back(data);
			}
		}
	}

	void DataInfoManager::clear()
	{
		for (VectorDataInfo::iterator data = mDataInfos.begin(); data != mDataInfos.end(); data ++)
			delete *data;
		mDataInfos.clear();
	}

	DataInfo* DataInfoManager::getData(const std::string& _type)
	{
		for (VectorDataInfo::const_iterator data = mDataInfos.begin(); data != mDataInfos.end(); data ++)
		{
			if ((*data)->getType() == _type)
				return *data;
		}

		return nullptr;
	}
}
